
#include <random>

#include "Tetromino.h"
#include "TetrominoManager.h"
#include <cassert>

class Vector2;

const unsigned int TetrominoManager::GRID_ROW_SIZE = 20U + WALL_ROW_SIZE;
const unsigned int TetrominoManager::GRID_COL_SIZE = 6U + WALL_COL_SIZE;

const unsigned int TetrominoManager::MAX_NEXT_TETROMINOS_COUNT = 40U;
const unsigned int TetrominoManager::MAX_NEXT_TETROMINOS_SHOW_COUNT = 5U;

TetrominoManager::TetrominoManager(Vector2 leftTopPosition)
    : GraphicsGrid(leftTopPosition, GRID_ROW_SIZE, GRID_COL_SIZE)
{
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<unsigned int> dis(0, Tetromino::TYPES_COUNT - 1);

    for (unsigned int i = 0; i < MAX_NEXT_TETROMINOS_COUNT; ++i)
    {
        unsigned int randomNumber = dis(gen);
        eTetrominoType type = static_cast<eTetrominoType>(randomNumber);

        mNextTetrominoList.push_back(new Tetromino(type));
    }
}

TetrominoManager::~TetrominoManager()
{
    while (mNextTetrominoList.empty() == false)
    {
        delete mNextTetrominoList.front();
        mNextTetrominoList.pop_front();
    }
}

void TetrominoManager::Update()
{
    // Todo: Reset grid
    {
        for (int row = 1; row < mGridRowSize - 1; ++row)
        {
            for (int col = 1; col < mGridColSize - 1; ++col)
            {
                mbGrid[row][col] = false;
            }
        }
    }

    // Todo: magic number
    // Todo: Holder row size
    // Todo: Modify class Position, setPosition()
    const unsigned int START_POSITION_ROW = 2U;
    const unsigned int START_POSITION_COL = 2U;
    const unsigned int ADD_OFFSET_ROW = 4U;

    Position addOffset(ADD_OFFSET_ROW, 0);
    Position moveOffset(START_POSITION_ROW, START_POSITION_COL);

    unsigned int showCount = 0;
    for (const Tetromino* tetromino : mNextTetrominoList)
    { 
        if (showCount == MAX_NEXT_TETROMINOS_SHOW_COUNT)
        {
            break;
        }

        for (const Position& blockPosition : tetromino->GetBlockPositions())
        {
            Position newBlockPosition = blockPosition + moveOffset;
            mbGrid[newBlockPosition.GetRow()][newBlockPosition.GetCol()] = true;
        }

        moveOffset += addOffset;
        showCount++;
    }
}

/*
void TetrominoManager::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    GraphicsGrid::Render(windowDeviceContext, memoryDeviceContext, windowResolution);

    // Todo: Render next list
    const unsigned int TETROMINO_ROW_SIZE = 3U;
    
    
}
*/

void TetrominoManager::Release(Tetromino* tetromino)
{
    assert(tetromino != nullptr);
    
    // Todo: 현재 들고 있는 메모리인지 확인해야함

    tetromino->ResetStates();
    mNextTetrominoList.push_back(tetromino);
}

Tetromino* TetrominoManager::GetNextTetromino()
{
    Tetromino* outTetromino = mNextTetrominoList.front();
    mNextTetrominoList.pop_front();

    return outTetromino;
}

