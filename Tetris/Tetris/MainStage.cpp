
#include <cassert>
#include <string>

#include "MainStage.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Tetromino.h"

#include "TetrominoManager.h"
#include "HoldManager.h"

const unsigned int MainStage::STAGES_COUNT = 5U;
const unsigned int MainStage::STAGE_LEVEL_REACH_SCORES[] =
{
    3, 6, 10, 15
};

//const float Grid::BLOCK_LENGTH = 30.f;
//const float Grid::GRID_WIDTH = GRID_COL_SIZE * BLOCK_LENGTH;
//const float Grid::GRID_HEIGHT = GRID_ROW_SIZE * BLOCK_LENGTH;

MainStage::MainStage(Vector2 leftTopVector, Tetromino* tetromino)
    : GraphicsGrid(leftTopVector, GRID_ROW_SIZE, GRID_COL_SIZE)
    , mTetromino(tetromino) // Todo: mTetromino to nullptr
{
    // Todo: 시간 결합 문제
    spawnTetromino();
}

MainStage::~MainStage()
{
    for (int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        delete[] mbGrid[row]; 
    }
    delete[] mbGrid;
}

void MainStage::Update(TetrominoManager* tetrominoManager)
{
    assert(mTetromino != nullptr);
    
    // Update tetromino
    {
        bool bTetrominoAlive = true;
        if (TimeManager::GetInstance()->HasTicked())
        {
            TimeManager::GetInstance()->ResetTick();

            bTetrominoAlive = mTetromino->MoveOneStep(eDirection::Down, *this);
        }
        else
        {
            // Todo: Maybe change to switch case
            KeyManager* keyManager = KeyManager::GetInstance();
            if (keyManager->GetKeyState(eKey::Left) == eKeyState::Press)
            {
                // Todo: bool MoveTetrominoOneStep();
                mTetromino->MoveOneStep(eDirection::Left, *this);
            }
            else if (keyManager->GetKeyState(eKey::Right) == eKeyState::Press)
            {
                mTetromino->MoveOneStep(eDirection::Right, *this);
            }
            else if (keyManager->GetKeyState(eKey::Up) == eKeyState::Press)
            {
                mTetromino->RotateCW(*this);
            }
            else if (keyManager->GetKeyState(eKey::Space) == eKeyState::Press)
            {
                while (bTetrominoAlive)
                {
                    bTetrominoAlive = mTetromino->MoveOneStep(eDirection::Down, *this);
                }

                assert(bTetrominoAlive == false);
            }
            else if (keyManager->GetKeyState(eKey::A) == eKeyState::Press)
            {
                if (mbUsedHold == false)
                {
                    assert(mTetromino != nullptr);

                    if (mHoldTetrominoOrNull == nullptr)
                    {
                        mHoldTetrominoOrNull = mTetromino;

                        mTetromino = tetrominoManager->GetNextTetromino();
                        spawnTetromino();
                    }
                    else
                    {
                        mTetromino = mHoldTetrominoOrNull;
                        spawnTetromino();
                        mHoldTetrominoOrNull = nullptr;

                        mbUsedHold = true;
                    }
                }
            }
        }

        // Todo: 상호참조 없애기
        if (bTetrominoAlive == false)
        {
            // Mark dead tetromino on grid 
            for (const Position& blockPosition : mTetromino->GetBlockPositions())
            {
                mbGrid[blockPosition.GetRow()][blockPosition.GetCol()] = true;
            }

            // Tetromino* ProvideNextTetromino();
            {
                tetrominoManager->Release(mTetromino);

                mTetromino = tetrominoManager->GetNextTetromino();
                spawnTetromino();
            }
        }
    }
    
    // Destroy lines and update score, stage level
    {
        unsigned int lineDestroyCount = 0;

        for (unsigned int row = SPAWN_ZONE_ROW_SIZE; row < GRID_ROW_SIZE - 1; ++row)
        {
            bool bLineFull = true;

            for (unsigned int col = 1; col < GRID_COL_SIZE - 1; ++col)
            {
                if (mbGrid[row][col] == false)
                {
                    bLineFull = false;

                    break;
                }
            }

            if (bLineFull == false)
            {
                continue;
            }

            ++lineDestroyCount;
            
            for (unsigned int copyRow = row; copyRow >= SPAWN_ZONE_ROW_SIZE; --copyRow)
            {
                for (unsigned int copyCol = 1; copyCol < GRID_COL_SIZE - 1; ++copyCol)
                {
                    mbGrid[copyRow][copyCol] = mbGrid[copyRow - 1][copyCol];
                }
            }
        }

        unsigned int addScore = 0;
        switch (lineDestroyCount)
        {
        case 0:
            addScore = 0;
            break;
        case 1:
            addScore = 1;
            break;
        case 2:
            addScore = 4;
            break;
        case 3:
            addScore = 6;
            break;
        case 4:
            addScore = 12;
            break;
        default:
            assert(false);
        }

        mTotalScore += addScore;

        unsigned int level = STAGES_COUNT;
        for (unsigned int levelIndex = 0; levelIndex < STAGES_COUNT - 1; ++levelIndex)
        {
            if (mTotalScore <= STAGE_LEVEL_REACH_SCORES[levelIndex])
            {
                level = levelIndex + 1;

                break;
            }
        }

        mStageLevel = level;
    }

}

// Todo: HDC 인자 중 하나는 전달할 필요 없음
void MainStage::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    GraphicsGrid::Render(windowDeviceContext, memoryDeviceContext, windowResolution);

    // Draw tetromino
    {
        for (const Position& blockPosition : mTetromino->GetBlockPositions())
        {
            int renderStartY = (blockPosition.GetRow() * BLOCK_LENGTH);
            int renderStartX = (blockPosition.GetCol() * BLOCK_LENGTH);

            Rectangle(memoryDeviceContext,
                renderStartX,
                renderStartY,
                renderStartX + BLOCK_LENGTH,
                renderStartY + BLOCK_LENGTH);
        }
    }

    // Draw strings
    {
        std::wstring printScore = L"Score: " + std::to_wstring(mTotalScore);
        std::wstring printLevel = L"Stage Level: " + std::to_wstring(mStageLevel);

        // Todo: No magic number, move position
        const float GRID_HEIGHT = mGridRowSize * BLOCK_LENGTH;
        const float PRINT_OFFSET = 10.f;
        const float PRINT_STRING_OFFSET = 30.f;
        
        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET, printScore.c_str(), printScore.length());
        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET + PRINT_STRING_OFFSET, printLevel.c_str(), printLevel.length());
    }

    /*
    BitBlt(windowDeviceContext, 0, 0, windowResolution.x, windowResolution.y,
        memoryDeviceContext, 0, 0, SRCCOPY);
        */
}

void MainStage::spawnTetromino()
{
    assert(mTetromino != nullptr);

    // Todo: Magic number
    mTetromino->ResetStates();
    mTetromino->MovePosition({ SPAWN_TETROMINO_ROW, SPAWN_TETROMINO_COL }, *this);
}

const bool* const* MainStage::GetGrid() const
{
    return mbGrid;
}

