
#include <cassert>
#include <string>

#include "GamePlayStage.h"
#include "KeyManager.h"
#include "TickTimer.h"

GamePlayStage::GamePlayStage()
    : GameStage({ 300.f, 0 }) // Todo: Magic number
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

void GamePlayStage::Update(double deltaTime)
{
    bool bTetrominoAlive = true;

    mTickTimer->AccumulatedTime(deltaTime);
    if (mGameStats->Ticked(mTickTimer))
    {
        bTetrominoAlive = mMainBoard->MoveTetrominoOneStep(eDirection::Down);
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
        return;
    }

    mMainBoard->LockDownTetromino(mTetrominoManager);

    unsigned int clearLinesCount = mMainBoard->ClearFullLines();
    if (clearLinesCount > 0)
    {
        mGameStats->UpdateInformationsFrom(clearLinesCount);
    }

    /*
    if (mGameStats->HasStageLevelUp())
    {
        //mMainBoard->mbUsedHold = true;

        // Todo: 속도 변경 등
    }
    */
}

void GamePlayStage::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    // Todo: Move to static
    const float GRID_HEIGHT = MainBoard::GRID_ROW_SIZE * BLOCK_LENGTH;
    const float GRID_WIDTH = MainBoard::GRID_COL_SIZE * BLOCK_LENGTH;

    const float BOX_LENGTH = 250.f;
    const float DRAW_OFFSET = 30.f;
    const float PRINT_STRING_OFFSET = 30.f;

    // Draw main board
    {
        const bool* const* drawGrid = mMainBoard->GetGrid();

        for (unsigned int row = 0; row < MainBoard::GRID_ROW_SIZE; ++row)
        {
            for (unsigned int col = 0; col < MainBoard::GRID_COL_SIZE; ++col)
            {
                if (drawGrid[row][col] == false)
                {
                    continue;
                }

                int renderStartY = mLeftTopPosition.Y + (row * BLOCK_LENGTH);
                int renderStartX = mLeftTopPosition.X + (col * BLOCK_LENGTH);

                Rectangle(memoryDeviceContext,
                          renderStartX,
                          renderStartY,
                          renderStartX + BLOCK_LENGTH,
                          renderStartY + BLOCK_LENGTH);
            }

            // Todo: Code duplicate, draw tetromino
            drawTetromino(mLeftTopPosition, *mMainBoard->GetActiveTetromino(), memoryDeviceContext);
        }
    }

    // Draw hold
    Vector2 holdBoxStartVector = { mLeftTopPosition.X - DRAW_OFFSET - BOX_LENGTH, 0.f };
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
    Vector2 infoBoxStartVector = { mLeftTopPosition.X - DRAW_OFFSET - BOX_LENGTH, holdBoxStartVector.Y + BOX_LENGTH + DRAW_OFFSET };
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

    // Draw next
    Vector2 nextBoxStartVector = { mLeftTopPosition.X + GRID_WIDTH + DRAW_OFFSET, mLeftTopPosition.Y };
    {
        Rectangle(memoryDeviceContext,
            nextBoxStartVector.X,
            nextBoxStartVector.Y,
            nextBoxStartVector.X + BOX_LENGTH - (BLOCK_LENGTH * 2),
            nextBoxStartVector.Y + (GRID_HEIGHT - (BLOCK_LENGTH * 2)));

        // Todo: Magic number
        Vector2 nextTetrominoStartVector = { nextBoxStartVector.X + (BOX_LENGTH / 2) - (BLOCK_LENGTH * 3), (BLOCK_LENGTH * 2) };//nextBoxStartVector.Y + (BOX_LENGTH / 2) - BLOCK_LENGTH };
        
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
