
#include <cassert>
#include <string>

#include "GamePlayStage.h"
#include "KeyManager.h"
#include "TickTimer.h"

const float GamePlayStage::BLOCK_LENGTH = 30.f;
const double GamePlayStage::SOFT_DROP_SPEED_DIVISOR = 5.0;

GamePlayStage::GamePlayStage()
    : GameStage({ 0.f, 0.f })
    , mTetrominoManager(new TetrominoManager({ 500.f, 0.f }))
    //, mMainBoard(new MainBoard(mTetrominoManager->GetNextTetromino()))
    , mGameStats(new GameStats())
    , mTickTimer(new TickTimer())
{
    mMainBoard = new MainBoard(mTetrominoManager->GetNextTetromino());
}

GamePlayStage::~GamePlayStage()
{
    delete mMainBoard;
    delete mTetrominoManager;
    delete mGameStats;
    delete mTickTimer;
}

GameResult GamePlayStage::GetGameResult() const
{
    return mGameStats->GetResult();
}

eStageType GamePlayStage::Update(double deltaTime)
{
    bool bTetrominoAlive = true;

    double tickRate = mGameStats->GetTickRate();
    mTickTimer->AccumulatedTime(deltaTime);

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
            tickRate /= SOFT_DROP_SPEED_DIVISOR;
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

    if (mTickTimer->GetAccumulatedTime() >= tickRate)
    {
        bTetrominoAlive = mMainBoard->MoveTetrominoOneStep(eDirection::Down);

        mTickTimer->ResetTimer();
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

    // Todo: Çò°¥¸²
    mMainBoard->ReleaseActiveTetromino(mTetrominoManager);
    mMainBoard->SetNextTetrominoFrom(mTetrominoManager);

    unsigned int clearLinesCount = mMainBoard->ClearFullLines();
    if (clearLinesCount > 0)
    {
        mGameStats->UpdateInformationsFrom(clearLinesCount);
    }

    return eStageType::Play;
}

void GamePlayStage::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    // Todo: Move to class static
    static const unsigned int WALL_SIZE = 2U;

    static const unsigned int GRID_ROW_SIZE = 20U + 2U + WALL_SIZE; // SPAWN
    static const unsigned int GRID_COL_SIZE = 10U + WALL_SIZE;

    static const float GRID_HEIGHT = GRID_ROW_SIZE * BLOCK_LENGTH;
    static const float GRID_WIDTH = GRID_COL_SIZE * BLOCK_LENGTH;

    static const float BOX_LENGTH = 250.f;
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

        const Tetromino* holdTetrominoOrNull = mMainBoard->GetHoldTetrominoOrNull();
        if (holdTetrominoOrNull != nullptr)
        {
            // Todo: duplicate
            Vector2 holdTetrominoStartVector = { holdBoxStartVector.X + (BOX_LENGTH / 2) - BLOCK_LENGTH, (BOX_LENGTH / 2) - BLOCK_LENGTH };
            drawTetromino(holdTetrominoStartVector, *holdTetrominoOrNull, memoryDeviceContext);
        }
    }

    // Draw strings
    Vector2 infoBoxStartVector = { holdBoxStartVector.X, holdBoxStartVector.Y + BOX_LENGTH + DRAW_OFFSET };
    {
        Rectangle(memoryDeviceContext,
            infoBoxStartVector.X,
            infoBoxStartVector.Y,
            infoBoxStartVector.X + BOX_LENGTH,
            infoBoxStartVector.Y + BOX_LENGTH);

        std::wstring printScore = L"Score: " + std::to_wstring(mGameStats->GetTotalScore());
        std::wstring printLevel = L"Stage Level: " + std::to_wstring(mGameStats->GetStageLevel());

        // Todo: No magic number, move position
        TextOut(memoryDeviceContext, infoBoxStartVector.X + DRAW_OFFSET, infoBoxStartVector.Y + DRAW_OFFSET, printScore.c_str(), printScore.length());
        TextOut(memoryDeviceContext, infoBoxStartVector.X + DRAW_OFFSET, infoBoxStartVector.Y + (DRAW_OFFSET * 2), printLevel.c_str(), printLevel.length());
    }

    Vector2 wallStartVector = { holdBoxStartVector.X + BOX_LENGTH + DRAW_OFFSET, holdBoxStartVector.Y };
    {
        int leftWallStartX = wallStartVector.X;
        int rightWallStartX = wallStartVector.X + (BLOCK_LENGTH * (GRID_COL_SIZE - 1));
        for (unsigned int row = 0; row < GRID_ROW_SIZE; ++row)
        {
            int wallStartY = wallStartVector.Y + (row * BLOCK_LENGTH);

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

        int upWallStartY = wallStartVector.Y;
        int downWallStartY = wallStartVector.Y + (BLOCK_LENGTH * (GRID_ROW_SIZE - 1));
        for (unsigned int col = 0; col < GRID_COL_SIZE; ++col)
        {
            int wallStartX = wallStartVector.X + (col * BLOCK_LENGTH);

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

                int renderStartX = mainBoardStartVector.X + (col * BLOCK_LENGTH);
                int renderStartY = mainBoardStartVector.Y + (row * BLOCK_LENGTH);

                Rectangle(memoryDeviceContext,
                    renderStartX,
                    renderStartY,
                    renderStartX + BLOCK_LENGTH,
                    renderStartY + BLOCK_LENGTH);
            }

            // Todo: Code duplicate, draw tetromino
            drawTetromino(mainBoardStartVector, *mMainBoard->GetActiveTetromino(), memoryDeviceContext);
        }
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

            drawTetromino(nextTetrominoStartVector, *nextTetromino, memoryDeviceContext);

            nextTetrominoStartVector.Y += (DRAW_OFFSET * 4);

            drawCount--;
        }
    }
}

void GamePlayStage::drawTetromino(Vector2 startVector, const Tetromino& tetromino, HDC memoryDeviceContext)
{
    for (const Position& blockPosition : tetromino.GetBlockPositions())
    {
        int renderStartY = startVector.Y + (blockPosition.GetRow() * BLOCK_LENGTH);
        int renderStartX = startVector.X + (blockPosition.GetCol() * BLOCK_LENGTH);

        Rectangle(memoryDeviceContext,
                 renderStartX,
                 renderStartY,
                 renderStartX + BLOCK_LENGTH,
                 renderStartY + BLOCK_LENGTH);
    }
}
