
#include <cassert>
#include <string>

#include "Grid.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Tetromino.h"

#include "TetrominoManager.h"


const unsigned int Grid::STAGES_COUNT = 5U;
const unsigned int Grid::STAGE_LEVEL_REACH_SCORES[] =
{
    3, 6, 10, 15
};

const float Grid::BLOCK_LENGTH = 30.f;
const float Grid::GRID_WIDTH = GRID_COL_SIZE * BLOCK_LENGTH;
const float Grid::GRID_HEIGHT = GRID_ROW_SIZE * BLOCK_LENGTH;

Grid::Grid(Tetromino* tetromino)
    : GraphicsObject({ 0.f, 0.f }, GRID_WIDTH, GRID_HEIGHT)
    , mTetromino(tetromino) // Todo: mTetromino to nullptr
{
    mbGrid = new bool*[GRID_ROW_SIZE];
    for (int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        mbGrid[row] = new bool[GRID_COL_SIZE];

        memset(mbGrid[row], false, sizeof(bool) * GRID_COL_SIZE);
    }

    // Todo: Make as unmoveable block
    for (unsigned int row = SPAWN_ZONE_ROW_SIZE; row < GRID_ROW_SIZE; ++row)
    {
        mbGrid[row][0] = true;
        mbGrid[row][GRID_COL_SIZE - 1] = true;
    }

    for (unsigned int col = 0; col < GRID_COL_SIZE; ++col)
    {
        mbGrid[GRID_ROW_SIZE - 1][col] = true;
    }

    // Todo: 시간 결합 문제
    spawnTetromino();
}

Grid::~Grid()
{
    for (int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        delete[] mbGrid[row]; 
    }
    delete[] mbGrid;

    while (mNextTetrominoList.empty() == false)
    {
        delete mNextTetrominoList.front();
        mNextTetrominoList.pop_front();
    }

    delete mTetromino;
}

void Grid::Update(TetrominoManager* tetrominoManager)
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
        }

        if (bTetrominoAlive == false)
        {
            // Mark dead tetromino on grid 
            for (const Position& blockPosition : mTetromino->GetBlockPositions())
            {
                mbGrid[blockPosition.GetRow()][blockPosition.GetCol()] = true;
            }

            tetrominoManager->Release(mTetromino);
            mTetromino = tetrominoManager->GetNextTetromino();

            spawnTetromino();
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
void Grid::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    // Todo: 사각형 크기 수정 필요함, 
    // Todo: 전체 도화지가 지워지기 때문에, 높은 레벨로 올려야함

    // Draw grid
    {
        Rectangle(memoryDeviceContext, -1, -1, windowResolution.x + 1, windowResolution.y + 1);

        for (unsigned int row = 0; row < GRID_ROW_SIZE; ++row)
        {
            for (unsigned int col = 0; col < GRID_COL_SIZE; ++col)
            {
                if (mbGrid[row][col] == false)
                {
                    continue;
                }

                int renderStartY = row * BLOCK_LENGTH;
                int renderStartX = col * BLOCK_LENGTH;

                Rectangle(memoryDeviceContext,
                    renderStartX,
                    renderStartY,
                    renderStartX + BLOCK_LENGTH,
                    renderStartY + BLOCK_LENGTH);
            }
        }

        for (const Position& blockPosition : mTetromino->GetBlockPositions())
        {
            int renderStartY = blockPosition.GetRow() * BLOCK_LENGTH;
            int renderStartX = blockPosition.GetCol() * BLOCK_LENGTH;

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
        const float PRINT_OFFSET = 10.f;
        const float PRINT_STRING_OFFSET = 30.f;
        
        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET, printScore.c_str(), printScore.length());
        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET + PRINT_STRING_OFFSET, printLevel.c_str(), printLevel.length());
    }

    BitBlt(windowDeviceContext, 0, 0, windowResolution.x, windowResolution.y,
        memoryDeviceContext, 0, 0, SRCCOPY);
}

void Grid::spawnTetromino()
{
    assert(mTetromino != nullptr);

    // Todo: Magic number
    mTetromino->ResetStates();
    mTetromino->MovePosition({ SPAWN_TETROMINO_ROW, SPAWN_TETROMINO_COL }, *this);
}

const bool* const* Grid::GetGrid() const
{
    return mbGrid;
}

