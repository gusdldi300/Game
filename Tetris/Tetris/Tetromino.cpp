
#include "Tetromino.h"
#include "Grid.h"

// Todo: MOVE_OFFSETS
const Position Tetromino::ONE_STEP_MOVE_OFFSETS[] =
{
    { 0, 1 },
    { 1, 0 },
    { 0, -1 }
};

Tetromino::Tetromino(Position moveOffset)
    : mMoveOffset(moveOffset)
    , mRotationState(eRotationState::Degree0)
{
}

bool Tetromino::MoveOneStep(eDirection direction, const Grid& grid)
{
    unsigned int dirIndex = static_cast<unsigned int>(direction);

    for (const Position& blockPosition : GetBlockPositions())
    {
        Position nextPosition = blockPosition + ONE_STEP_MOVE_OFFSETS[dirIndex];
        if (canPlaceOnGrid(nextPosition, grid) == false)
        {
            return false;
        }
    }

    mMoveOffset += Tetromino::ONE_STEP_MOVE_OFFSETS[dirIndex];

    return true;
}

// Todo: Change to MovePositionOnGrid()
void Tetromino::MovePosition(Position position)
{
    // Todo: position assert

    mMoveOffset += position;
}

void Tetromino::ResetStates()
{
    mMoveOffset = { 0, 0 };
    mRotationState = eRotationState::Degree0;
}

bool Tetromino::canPlaceOnGrid(Position position, const Grid& grid) const
{
    // Todo: 경계가 헷갈림. 상수 수정 필요
    int positionRow = position.GetRow();
    int positionCol = position.GetCol();

    if (positionRow <= 0 || positionRow >= Grid::GRID_ROW_SIZE - 1 ||
        positionCol <= 0 || positionCol >= Grid::GRID_COL_SIZE - 1)
    {
        return false;
    }

    const bool* const* bGrid = grid.GetGrid();
    if (bGrid[positionRow][positionCol])
    {
        return false;
    }

    return true;
}