#include <cassert>

#include "HoldManager.h"
#include "Position.h"
#include "Tetromino.h"
#include "TetrominoManager.h"
#include "MainBoard.h"

const unsigned int HoldManager::GRID_ROW_SIZE = 4U + WALL_ROW_SIZE;
const unsigned int HoldManager::GRID_COL_SIZE = 6U + WALL_COL_SIZE;

HoldManager::HoldManager(Vector2 leftTopPosition)
    : GraphicsGrid(leftTopPosition, GRID_ROW_SIZE, GRID_COL_SIZE)
    , mbUsedHoldStage(false)
    , mHoldTetrominoOrNull(nullptr)
{
}

bool HoldManager::HasUsedHold() const
{
    return mbUsedHoldStage;
}

bool HoldManager::HasHold() const
{
    return (mHoldTetrominoOrNull == nullptr) ? false : true;
}

bool HoldManager::SetHold(Tetromino* holdTetromino)
{
    if (HasUsedHold() || HasHold())
    {
        return false;
    }

    mHoldTetrominoOrNull = holdTetromino;
    mHoldTetrominoOrNull->ResetStates();

    // Todo: Update grid, erase later
    {
        for (int row = 1; row < mGridRowSize - 1; ++row)
        {
            for (int col = 1; col < mGridColSize - 1; ++col)
            {
                mbGrid[row][col] = false;
            }
        }

        const unsigned int START_POSITION_ROW = 2U;
        const unsigned int START_POSITION_COL = 2U;

        Position moveOffset(START_POSITION_ROW, START_POSITION_COL);

        assert(mHoldTetrominoOrNull != nullptr);
        for (const Position& blockPosition : mHoldTetrominoOrNull->GetBlockPositions())
        {
            Position newBlockPosition = blockPosition + moveOffset;
            mbGrid[newBlockPosition.GetRow()][newBlockPosition.GetCol()] = true;
        }
    }

    return true;
}

Tetromino* HoldManager::UseHoldOrNull()
{
    if (HasUsedHold() || HasHold() == false)
    {
        return nullptr;
    }

    mbUsedHoldStage = true;

    Tetromino* pOutHold = mHoldTetrominoOrNull;
    mHoldTetrominoOrNull = nullptr;

    // Todo: Code duplicate, erase later
    {
        for (int row = 1; row < mGridRowSize - 1; ++row)
        {
            for (int col = 1; col < mGridColSize - 1; ++col)
            {
                mbGrid[row][col] = false;
            }
        }
    }

    return pOutHold;
}

/*
bool HoldManager::UseHold(MainBoard* mainBoard, TetrominoManager* terominoManager)
{
    // Todo: Compare with MainBoard.UseHold()
    if (mbUsedHoldStage == false)
    {
        //assert(mTetromino != nullptr);

        if (mHoldTetrominoOrNull == nullptr)
        {
            // Todo: Maybe tetrominoManager not needed
            mHoldTetrominoOrNull = mainBoard->GetTeromino();
            mainBoard->setTetroimino(terominoManager->GetNextTetromino());

            mainBoard->GetTeromino()->SpawnTetromino();
        }
        else
        {
            mainBoard->setTetroimino(mHoldTetrominoOrNull);
        }
    }

    return false;
}
*/
