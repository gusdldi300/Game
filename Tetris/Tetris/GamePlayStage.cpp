
#include <cassert>
#include <string>

#include "GamePlayStage.h"
#include "KeyManager.h"
#include "TickTimer.h"

const float GamePlayStage::DRAW_BOARD_HEIGHT = GamePlayStage::DRAW_BOARD_ROW_SIZE * Renderer::BLOCK_LENGTH;
const float GamePlayStage::DRAW_BOARD_WIDTH = GamePlayStage::DRAW_BOARD_COL_SIZE * Renderer::BLOCK_LENGTH;
const float GamePlayStage::BOX_HALF_LENGTH = (Renderer::BOX_LENGTH / 2);

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
    Renderer* renderer = Renderer::GetInstance();

    // Draw hold
    Vector2 holdBoxStartVector = { Renderer::DRAW_OFFSET, Renderer::DRAW_OFFSET };
    {
        assert(holdBoxStartVector.X >= 0.f);

        renderer->DrawRectangle(memoryDeviceContext,
            holdBoxStartVector.X,
            holdBoxStartVector.Y,
            holdBoxStartVector.X + Renderer::BOX_LENGTH,
            holdBoxStartVector.Y + Renderer::BOX_LENGTH);

        const Tetromino* holdTetrominoOrNull = mTetrominoManager->GetHoldTetrominoOrNull();
        if (holdTetrominoOrNull != nullptr)
        {
            Vector2 holdTetrominoStartVector = 
            { 
                holdBoxStartVector.X + BOX_HALF_LENGTH - Renderer::BLOCK_LENGTH,
                holdBoxStartVector.Y + BOX_HALF_LENGTH - Renderer::BLOCK_LENGTH
            };

            renderer->DrawBlocks(memoryDeviceContext, holdTetrominoStartVector, 
                static_cast<eColor>(holdTetrominoOrNull->GetColor()),
                holdTetrominoOrNull->GetBlockPositions());
        }
    }

    // Draw stats
    Vector2 infoBoxStartVector = { holdBoxStartVector.X, holdBoxStartVector.Y + Renderer::BOX_LENGTH + Renderer::DRAW_OFFSET };
    {
        // Todo: Make as function
        std::vector<std::wstring> drawStats;
        std::wstring scoreString = L"Score: " + std::to_wstring(mGameStats->GetTotalScore());
        std::wstring levelString = L"Stage Level: " + std::to_wstring(mGameStats->GetStageLevel());

        drawStats.push_back(scoreString);
        drawStats.push_back(levelString);

        renderer->DrawStatsBoard(memoryDeviceContext, infoBoxStartVector, drawStats);
    }

    // Draw walls
    Vector2 wallStartVector = { holdBoxStartVector.X + Renderer::BOX_LENGTH + Renderer::DRAW_OFFSET, holdBoxStartVector.Y };
    {
        // Todo: Reserve
        std::vector<Position> wallPositions;

        int leftWallCol = 0;
        int rightWallCol = DRAW_BOARD_COL_SIZE - 1;
        for (int row = 0; row < DRAW_BOARD_ROW_SIZE; ++row)
        {
            wallPositions.push_back({ row, leftWallCol });
            wallPositions.push_back({ row, rightWallCol });
        }

        int upWallRow = 0;
        int downWallRow = DRAW_BOARD_ROW_SIZE - 1;
        
        for (int col = 0; col < DRAW_BOARD_COL_SIZE; ++col)
        {
            wallPositions.push_back({ upWallRow, col });
            wallPositions.push_back({ downWallRow, col });
        }

        renderer->DrawBlocks(memoryDeviceContext, wallStartVector, eColor::LightBlack, wallPositions);
    }

    // Draw main board
    Vector2 mainBoardStartVector = { wallStartVector.X + Renderer::BLOCK_LENGTH, wallStartVector.Y + Renderer::BLOCK_LENGTH };
    {
        const eColor* const* drawGrid = mMainBoard->GetColorBoard();

        for (int row = 0; row < MainBoard::BOARD_ROW_SIZE; ++row)
        {
            for (int col = 0; col < MainBoard::BOARD_COL_SIZE; ++col)
            {
                if (drawGrid[row][col] == eColor::None)
                {
                    continue;
                }

                renderer->DrawBlock(memoryDeviceContext, mainBoardStartVector, drawGrid[row][col], { row, col });
            }
        }

        const Tetromino* activeTetromino = mMainBoard->GetActiveTetromino();
        renderer->DrawBlocks(memoryDeviceContext, mainBoardStartVector,
            activeTetromino->GetColor(),
            activeTetromino->GetBlockPositions());
        
        renderer->DrawBlocks(memoryDeviceContext, mainBoardStartVector, 
            eColor::Gray, 
            mMainBoard->GetGhostTetrominoBlockPositions());
    }

    // Draw next tetrominos
    Vector2 nextBoxStartVector = { wallStartVector.X + (Renderer::BLOCK_LENGTH * DRAW_BOARD_COL_SIZE) + Renderer::DRAW_OFFSET, wallStartVector.Y };
    {
        // Todo: Magic number
        renderer->DrawRectangle(memoryDeviceContext,
            nextBoxStartVector.X,
            nextBoxStartVector.Y,
            nextBoxStartVector.X + Renderer::BOX_LENGTH - (Renderer::BLOCK_LENGTH * 2),
            nextBoxStartVector.Y + (DRAW_BOARD_HEIGHT - (Renderer::BLOCK_LENGTH * 2)));

        Vector2 nextTetrominoStartVector = { nextBoxStartVector.X + BOX_HALF_LENGTH - (Renderer::BLOCK_LENGTH * 3), (Renderer::BLOCK_LENGTH * 2) };
        const std::list<Tetromino*> nextTetrominoList = mTetrominoManager->GetNextTetrominoList();
        
        unsigned int drawCount = DRAW_NEXT_TETROMINOS_COUNT;
        for (const Tetromino* nextTetromino : nextTetrominoList)
        {
            if (drawCount == 0)
            {
                break;
            }

            renderer->DrawBlocks(memoryDeviceContext, nextTetrominoStartVector, 
                nextTetromino->GetColor(),
                nextTetromino->GetBlockPositions());
            
            // Todo: Magic number
            nextTetrominoStartVector.Y += (Renderer::DRAW_OFFSET * 4);

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


