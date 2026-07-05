
#include <cassert>
#include <string>

#include "GamePlayStage.h"
#include "KeyManager.h"
#include "TickTimer.h"

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

    double fallTickRate = mGameStats->GetFallTickRate();
    mFallTickTimer->AccumulateTime(deltaTime);
    mRiseTickTimer->AccumulateTime(deltaTime);

    if (mGameStats->HasRiseTicked(mRiseTickTimer))
    {
        if (mMainBoard->RiseBlocksOneStep())
        {
            return eStageType::End;
        }

        mRiseTickTimer->ResetTimer();
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

    if (mFallTickTimer->GetAccumulatedTime() >= fallTickRate)
    {
        bTetrominoAlive = mMainBoard->MoveTetrominoOneStep(eDirection::Down);

        mFallTickTimer->ResetTimer();
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
    // Todo: Move to class static
    static const unsigned int WALL_SIZE = 2U;

    static const unsigned int GRID_ROW_SIZE = 20U + 2U + WALL_SIZE; // SPAWN
    static const unsigned int GRID_COL_SIZE = 10U + WALL_SIZE;

    static const float GRID_HEIGHT = GRID_ROW_SIZE * BLOCK_LENGTH;
    static const float GRID_WIDTH = GRID_COL_SIZE * BLOCK_LENGTH;

    static const float DRAW_OFFSET = 30.f;
    static const float PRINT_STRING_OFFSET = 30.f;

    // Draw hold
    Vector2 holdBoxStartVector = { DRAW_OFFSET, DRAW_OFFSET };
    {
        assert(holdBoxStartVector.X >= 0);

        Rectangle(memoryDeviceContext,
            holdBoxStartVector.X,
            holdBoxStartVector.Y,
            holdBoxStartVector.X + BOX_LENGTH,
            holdBoxStartVector.Y + BOX_LENGTH);

        const Tetromino* holdTetrominoOrNull = mTetrominoManager->GetHoldTetrominoOrNull();
        if (holdTetrominoOrNull != nullptr)
        {
            // Todo: duplicate
            Vector2 holdTetrominoStartVector = { holdBoxStartVector.X + (BOX_LENGTH / 2) - BLOCK_LENGTH, (BOX_LENGTH / 2) - BLOCK_LENGTH };
            drawTetrominoBlocks(memoryDeviceContext, holdTetrominoStartVector, holdTetrominoOrNull->GetBlockPositions());
        }
    }

    // Draw stats
    Vector2 infoBoxStartVector = { holdBoxStartVector.X, holdBoxStartVector.Y + BOX_LENGTH + DRAW_OFFSET };

    // Todo: Make as function
    std::vector<std::wstring> drawStats;
    std::wstring scoreString = L"Score: " + std::to_wstring(mGameStats->GetTotalScore());
    std::wstring levelString = L"Stage Level: " + std::to_wstring(mGameStats->GetStageLevel());

    drawStats.push_back(scoreString);
    drawStats.push_back(levelString);

    drawScores(memoryDeviceContext, infoBoxStartVector, drawStats);

    // Draw walls
    Vector2 wallStartVector = { holdBoxStartVector.X + BOX_LENGTH + DRAW_OFFSET, holdBoxStartVector.Y };
    {
        int leftWallStartX = static_cast<int>(wallStartVector.X);
        int rightWallStartX = static_cast<int>(wallStartVector.X + (BLOCK_LENGTH * (GRID_COL_SIZE - 1)));
        for (int row = 0; row < GRID_ROW_SIZE; ++row)
        {
            int wallStartY = static_cast<int>(wallStartVector.Y + (row * BLOCK_LENGTH));

            Rectangle(memoryDeviceContext,
                leftWallStartX,
                wallStartY,
                leftWallStartX + BLOCK_LENGTH,
                wallStartY + BLOCK_LENGTH);

            Rectangle(memoryDeviceContext,
                rightWallStartX,
                wallStartY,
                rightWallStartX + BLOCK_LENGTH,
                wallStartY + BLOCK_LENGTH);
        }

        int upWallStartY = static_cast<int>(wallStartVector.Y);
        int downWallStartY = static_cast<int>(wallStartVector.Y + (BLOCK_LENGTH * (GRID_ROW_SIZE - 1)));
        for (unsigned int col = 0; col < GRID_COL_SIZE; ++col)
        {
            int wallStartX = static_cast<int>(wallStartVector.X + (col * BLOCK_LENGTH));

            Rectangle(memoryDeviceContext,
                wallStartX,
                upWallStartY,
                wallStartX + BLOCK_LENGTH,
                upWallStartY + BLOCK_LENGTH);

            Rectangle(memoryDeviceContext,
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

                int renderStartX = static_cast<int>(mainBoardStartVector.X + (col * BLOCK_LENGTH));
                int renderStartY = static_cast<int>(mainBoardStartVector.Y + (row * BLOCK_LENGTH));

                Rectangle(memoryDeviceContext,
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
    Vector2 nextBoxStartVector = { wallStartVector.X + (BLOCK_LENGTH * GRID_COL_SIZE) + DRAW_OFFSET, wallStartVector.Y };
    {
        Rectangle(memoryDeviceContext,
            nextBoxStartVector.X,
            nextBoxStartVector.Y,
            nextBoxStartVector.X + BOX_LENGTH - (BLOCK_LENGTH * 2),
            nextBoxStartVector.Y + (GRID_HEIGHT - (BLOCK_LENGTH * 2)));

        // Todo: Magic number
        Vector2 nextTetrominoStartVector = { nextBoxStartVector.X + (BOX_LENGTH / 2) - (BLOCK_LENGTH * 3), (BLOCK_LENGTH * 2) }; //nextBoxStartVector.Y + (BOX_LENGTH / 2) - BLOCK_LENGTH };
        
        const std::list<Tetromino*> nextTetrominoList = mTetrominoManager->GetNextTetrominoList();

        unsigned int drawCount = 5;
        for (const Tetromino* nextTetromino : nextTetrominoList)
        {
            if (drawCount == 0)
            {
                break;
            }

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

void GamePlayStage::drawTetrominoBlocks(HDC memoryDeviceContext, Vector2 drawLeftTopVector, const std::vector<Position>& tetrominoBlocks)
{
    //std::vector<Position> blockPositions = tetromino.GetBlockPositions();

    for (const Position& blockPosition : tetrominoBlocks)
    {
        int renderStartY = static_cast<int>(drawLeftTopVector.Y + (blockPosition.GetRow() * BLOCK_LENGTH));
        int renderStartX = static_cast<int>(drawLeftTopVector.X + (blockPosition.GetCol() * BLOCK_LENGTH));

        Rectangle(memoryDeviceContext,
                 renderStartX,
                 renderStartY,
                 renderStartX + BLOCK_LENGTH,
                 renderStartY + BLOCK_LENGTH);
    }
}
