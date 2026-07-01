#include "HoldManager.h"
#include <cassert>

//const unsigned int HoldManager::GRID_ROW_SIZE = 4U + WALL_ROW_SIZE;
//const unsigned int HoldManager::GRID_COL_SIZE = 6U + WALL_COL_SIZE;

HoldManager::HoldManager(Vector2 leftTopPosition)
    //: GraphicsGrid(leftTopPosition, GRID_ROW_SIZE, GRID_COL_SIZE)
    //, mbUsedStage(false)
    //, mTetrominoOrNull(nullptr)
{
}

bool HoldManager::CanUseHold() const
{
    if (mTetrominoOrNull == nullptr || mbUsedStage)
    {
        return false;
    }

    return true;
}
