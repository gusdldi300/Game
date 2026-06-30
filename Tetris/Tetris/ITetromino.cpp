
#include "ITetromino.h"

const unsigned int ITetromino::BLOCKS_COUNT = 4U;

const std::vector<Position> ITetromino::BLOCK_POSITIONS[] = 
{
    { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0 ,3 } }, // Degree 0
    { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3 ,2 } },
    { { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2 ,3 } }, 
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3 ,1 } }
};

ITetromino::ITetromino(Position moveOffset)
    : Tetromino(moveOffset)
{
}

std::vector<Position> ITetromino::GetBlockPositions() const
{
    std::vector<Position> blockPositions;
    blockPositions.reserve(BLOCKS_COUNT);

    unsigned int rotationIndex = static_cast<unsigned int>(mRotationState);
    for (const Position& position : BLOCK_POSITIONS[rotationIndex])
    {
        blockPositions.push_back(position + mMoveOffset);
    }

    return blockPositions;
}

bool ITetromino::RotateCW(const Grid& grid)
{
    unsigned int nextRotationStateIndex = (static_cast<unsigned int>(mRotationState) + 1);
    nextRotationStateIndex %= static_cast<unsigned int>(eRotationState::End);

    for (const Position& position : BLOCK_POSITIONS[nextRotationStateIndex])
    {
        Position nextPosition = position + mMoveOffset;
        
        if (canPlaceOnGrid(nextPosition, grid) == false)
        {
            return false;
        }
    }

    mRotationState = static_cast<eRotationState>(nextRotationStateIndex);

    return true;
}

