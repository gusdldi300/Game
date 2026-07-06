
#include <cassert>
#include <string>

#include "GamePlayStage.h"
#include "KeyManager.h"
#include "TickTimer.h"

const float GamePlayStage::DRAW_BOARD_HEIGHT = DRAW_BOARD_ROW_SIZE * BLOCK_LENGTH;
const float GamePlayStage::DRAW_BOARD_WIDTH = DRAW_BOARD_COL_SIZE * BLOCK_LENGTH;
const float GamePlayStage::BOX_HALF_LENGTH = (BOX_LENGTH / 2);

const float GamePlayStage::BLOCK_LENGTH = 30.f;
const double GamePlayStage::SOFT_DROP_SPEED_DIVISOR = 5.0;

GamePlayStage::GamePlayStage()
    : GameStage({ 0.f, 0.f })
    , mTetrominoManager(new TetrominoManager())
    , mGameStats(new GameStat())
    , mFallTickTimer(new TickTimer())
    , mRiseTickTimer(new TickTimer())
{
    mMainBoard = new MainBoard(mTetrominoManager->ProvideNextTetromino());
}

GamePlayStage::~GamePlayStage()
{
    delete mMainBoard;
    delete mTetrominoManager;
    delete mGameStats;
    delete mFallTickTimer;
    delete mRiseTickTimer;
}

GameResult GamePlayStage::GetGameResult() const
{
    return mGameStats->GetResult();
}

eStageType GamePlayStage::Update(double deltaTime)
{
    bool bTetrominoAlive = true;

    // Update timer events
    double fallTickRate = mGameStats->GetFallTickRate();
    {
        mFallTickTimer->AccumulateTime(deltaTime);
        mRiseTickTimer->AccumulateTime(deltaTime);

        if (mFallTickTimer->GetAccumulatedTime() >= fallTickRate)
        {
            bTetrominoAlive = mMainBoard->MoveTetrominoOneStep(eDirection::Down);

            mFallTickTimer->ResetTimer();
        }

        if (mGameStats->HasRiseTicked(mRiseTickTimer))
        {
            if (mMainBoard->RiseBlocksOneStep())
            {
                return eStageType::End;
            }

            mRiseTickTimer->ResetTimer();
        }
    }

    // Move tetromino on board
    {
        // Todo: Maybe change to switch case
        KeyManager* keyManager = KeyManager::GetInstance();
        if (keyManager->GetKeyState(eKey::Left) == eKeyState::Press)
        {
            mMainBoard->MoveTetrominoOneStep(eDirection::Left);
        }
        else if (keyManager->GetKeyState(eKey::Right) == eKeyState::Press)
        {
            mMainBoard->MoveTetrominoOneStep(eDirection::Right);
        }
        else if (keyManager->GetKeyState(eKey::Down) == eKeyState::Hold)
        {
            fallTickRate /= SOFT_DROP_SPEED_DIVISOR;
        }
        else if (keyManager->GetKeyState(eKey::Up) == eKeyState::Press)
        {
            mMainBoard->RotateTetrominoCW();
        }
        else if (keyManager->GetKeyState(eKey::Space) == eKeyState::Press)
        {
            while (bTetrominoAlive)
            {
                bTetrominoAlive = mMainBoard->MoveTetrominoOneStep(eDirection::Down);
            }

            assert(bTetrominoAlive == false);
        }
        else if (keyManager->GetKeyState(eKey::A) == eKeyState::Press)
        {
            mMainBoard->AddHold(mTetrominoManager);
        }
        else if (keyManager->GetKeyState(eKey::S) == eKeyState::Press)
        {
            mMainBoard->UseHold(mTetrominoManager);
        }
    }

    if (bTetrominoAlive)
    {
        return eStageType::Play;
    }

    mMainBoard->LockDownTetromino(mTetrominoManager);
    if (mMainBoard->IsGameOver())
    {
        return eStageType::End;
    }

    mMainBoard->ReleaseAndSetNextActiveTetrominoFrom(mTetrominoManager);

    unsigned int clearLinesCount = mMainBoard->ClearFullLines();
    if (clearLinesCount > 0)
    {
        unsigned int lastLevel = mGameStats->GetStageLevel();
        mGameStats->UpdateInformationsFrom(clearLinesCount);

        if (lastLevel < mGameStats->GetStageLevel())
        {
            mTetrominoManager->ResetHold();
        }
    }

    return eStageType::Play;
}

void GamePlayStage::Render(HDC memoryDeviceContext)
{
    // Draw hold
    Vector2 holdBoxStartVector = { DRAW_OFFSET, DRAW_OFFSET };
    {
        assert(holdBoxStartVector.X >= 0.f);

        drawRectangle(memoryDeviceContext, 
            holdBoxStartVector.X,
            holdBoxStartVector.Y,
            holdBoxStartVector.X + BOX_LENGTH, 
            holdBoxStartVector.Y + BOX_LENGTH);

        const Tetromino* holdTetrominoOrNull = mTetrominoManager->GetHoldTetrominoOrNull();
        if (holdTetrominoOrNull != nullptr)
        {
            Vector2 holdTetrominoStartVector = 
            { 
                holdBoxStartVector.X + BOX_HALF_LENGTH - BLOCK_LENGTH, 
                holdBoxStartVector.Y + BOX_HALF_LENGTH - BLOCK_LENGTH 
            };

            drawTetrominoBlocks(memoryDeviceContext, holdTetrominoStartVector, holdTetrominoOrNull->GetBlockPositions());
        }
    }

    // Draw stats
    Vector2 infoBoxStartVector = { holdBoxStartVector.X, holdBoxStartVector.Y + BOX_LENGTH + DRAW_OFFSET };
    {
        // Todo: Make as function
        std::vector<std::wstring> drawStats;
        std::wstring scoreString = L"Score: " + std::to_wstring(mGameStats->GetTotalScore());
        std::wstring levelString = L"Stage Level: " + std::to_wstring(mGameStats->GetStageLevel());

        drawStats.push_back(scoreString);
        drawStats.push_back(levelString);

        drawStatsBoard(memoryDeviceContext, infoBoxStartVector, drawStats);
    }

    // Draw walls
    Vector2 wallStartVector = { holdBoxStartVector.X + BOX_LENGTH + DRAW_OFFSET, holdBoxStartVector.Y };
    {
        float leftWallStartX = wallStartVector.X;
        float rightWallStartX = wallStartVector.X + (BLOCK_LENGTH * (DRAW_BOARD_COL_SIZE - 1));
        for (int row = 0; row < DRAW_BOARD_ROW_SIZE; ++row)
        {
            float wallStartY = wallStartVector.Y + (row * BLOCK_LENGTH);

            drawRectangle(memoryDeviceContext,
                leftWallStartX,
                wallStartY,
                leftWallStartX + BLOCK_LENGTH,
                wallStartY + BLOCK_LENGTH);

            drawRectangle(memoryDeviceContext,
                rightWallStartX,
                wallStartY,
                rightWallStartX + BLOCK_LENGTH,
                wallStartY + BLOCK_LENGTH);
        }

        float upWallStartY = wallStartVector.Y;
        float downWallStartY = wallStartVector.Y + (BLOCK_LENGTH * (DRAW_BOARD_ROW_SIZE - 1));
        for (unsigned int col = 0; col < DRAW_BOARD_COL_SIZE; ++col)
        {
            float wallStartX = wallStartVector.X + (col * BLOCK_LENGTH);

            drawRectangle(memoryDeviceContext,
                wallStartX,
                upWallStartY,
                wallStartX + BLOCK_LENGTH,
                upWallStartY + BLOCK_LENGTH);

            drawRectangle(memoryDeviceContext,
                wallStartX,
                downWallStartY,
                wallStartX + BLOCK_LENGTH,
                downWallStartY + BLOCK_LENGTH);
        }
    }

    // Draw main board
    Vector2 mainBoardStartVector = { wallStartVector.X + BLOCK_LENGTH, wallStartVector.Y + BLOCK_LENGTH };
    {
        const bool* const* drawGrid = mMainBoard->GetGrid();

        for (unsigned int row = 0; row < MainBoard::BOARD_ROW_SIZE; ++row)
        {
            for (unsigned int col = 0; col < MainBoard::BOARD_COL_SIZE; ++col)
            {
                if (drawGrid[row][col] == false)
                {
                    continue;
                }

                float renderStartX = mainBoardStartVector.X + (col * BLOCK_LENGTH);
                float renderStartY = mainBoardStartVector.Y + (row * BLOCK_LENGTH);

                drawRectangle(memoryDeviceContext,
                    renderStartX,
                    renderStartY,
                    renderStartX + BLOCK_LENGTH,
                    renderStartY + BLOCK_LENGTH);
            }
        }

        drawTetrominoBlocks(memoryDeviceContext, mainBoardStartVector, mMainBoard->GetActiveTetromino()->GetBlockPositions());
        
        std::vector<Position> ghostTetrominoBlocks = mMainBoard->GetGhostTetrominoBlockPositions();
        drawTetrominoBlocks(memoryDeviceContext, mainBoardStartVector, ghostTetrominoBlocks);
    }

    // Draw next tetrominos
    Vector2 nextBoxStartVector = { wallStartVector.X + (BLOCK_LENGTH * DRAW_BOARD_COL_SIZE) + DRAW_OFFSET, wallStartVector.Y };
    {
        // Todo: Magic number
        drawRectangle(memoryDeviceContext,
            nextBoxStartVector.X,
            nextBoxStartVector.Y,
            nextBoxStartVector.X + BOX_LENGTH - (BLOCK_LENGTH * 2),
            nextBoxStartVector.Y + (DRAW_BOARD_HEIGHT - (BLOCK_LENGTH * 2)));

        Vector2 nextTetrominoStartVector = { nextBoxStartVector.X + BOX_HALF_LENGTH - (BLOCK_LENGTH * 3), (BLOCK_LENGTH * 2) };
        
        const std::list<Tetromino*> nextTetrominoList = mTetrominoManager->GetNextTetrominoList();

        unsigned int drawCount = DRAW_NEXT_TETROMINOS_COUNT;
        for (const Tetromino* nextTetromino : nextTetrominoList)
        {
            if (drawCount == 0)
            {
                break;
            }

            // Todo: Magic number
            drawTetrominoBlocks(memoryDeviceContext, nextTetrominoStartVector, nextTetromino->GetBlockPositions());
            nextTetrominoStartVector.Y += (DRAW_OFFSET * 4);

            drawCount--;
        }
    }
}

void GamePlayStage::ResetGame()
{
    mMainBoard->Reset();;
    mTetrominoManager->Reset();
    mGameStats->Reset();
    mFallTickTimer->ResetTimer();
    mRiseTickTimer->ResetTimer();
}

void GamePlayStage::drawTetrominoBlocks(HDC memoryDeviceContext, const Vector2& leftTopVector, const std::vector<Position>& tetrominoBlocks)
{
    //std::vector<Position> blockPositions = tetromino.GetBlockPositions();

    for (const Position& blockPosition : tetrominoBlocks)
    {
        float drawLeftTopX = leftTopVector.X + (blockPosition.GetCol() * BLOCK_LENGTH);
        float drawLeftTopY = leftTopVector.Y + (blockPosition.GetRow() * BLOCK_LENGTH);

        drawRectangle(memoryDeviceContext,
            drawLeftTopX,
            drawLeftTopY,
            drawLeftTopX + BLOCK_LENGTH, 
            drawLeftTopY + BLOCK_LENGTH);
    }
}
