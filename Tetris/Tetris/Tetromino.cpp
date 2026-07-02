
#include "MainBoard.h"
#include "Tetromino.h"

const unsigned int Tetromino::TYPES_COUNT = 7U;
const unsigned int Tetromino::ROTATATION_STATES_COUNT = 4U;

// Todo: Check s tetromino, akward rotation
// Todo: magic number
const std::vector<Position> Tetromino::BLOCK_POSITIONS[][4] =
{
    // 1. I 블록
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } }, // Degree 0
        { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 } },
        { { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } }
    },

    // 2. O 블록
    {
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
    },

    // 3. T 블록
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 1 } },
        { { 0, 2 }, { 1, 1 }, { 1, 2 }, { 2, 2 } }, 
        { { 1, 1 }, { 2, 0 }, { 2, 1 }, { 2, 2 } },
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 0 } } 
    },

    // 4. L 블록
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 0 } },
        { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 } },
        { { 0, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 } },
        { { 1, 2 }, { 2, 0 }, { 2, 1 }, { 2, 2 } }
    },

    // 5. J_블록
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 2 } },
        { { 0, 2 }, { 1, 2 }, { 2, 1 }, { 2, 2 } },
        { { 1, 0 }, { 2, 0 }, { 2, 1 }, { 2, 2 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 2, 0 } }
    },

    // 6. S 블록
    {
        { { 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 1 } }, 
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
        { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 1, 1 }, { 1, 2 }, { 2, 0 }, { 2, 1 } }
    },

    // 7. Z 블록
    {
        { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
        { { 0, 2 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
        { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 0 } }
    }
};

Tetromino::Tetromino(eTetrominoType type)
    : mMoveOffset({ 0, 0 })
    , mRotationState(eRotationState::Degree0)
    , mType(type)
{
}

Position Tetromino::GetPositionMoveOffset() const
{
    return mMoveOffset;
}

void Tetromino::MovePosition(Position position)
{
    mMoveOffset = position;
}

std::vector<Position> Tetromino::GetBlockPositions() const
{
    return GetRotatedBlockPositions(mRotationState);
}

std::vector<Position> Tetromino::GetRotatedBlockPositions(eRotationState rotationState) const
{
    std::vector<Position> blockPositions;
    //blockPositions.reserve(BLOCKS_COUNT);

    unsigned int typeIndex = static_cast<unsigned int>(mType);
    unsigned int rotationIndex = static_cast<unsigned int>(rotationState);
    for (const Position& position : BLOCK_POSITIONS[typeIndex][rotationIndex])
    {
        blockPositions.push_back(position + mMoveOffset);
    }

    return blockPositions;
}

eRotationState Tetromino::GetRotationState() const
{
    return mRotationState;
}

eTetrominoType Tetromino::GetType() const
{
    return mType;
}

void Tetromino::RotateCW()
{
    unsigned int nextRotationStateIndex = (static_cast<unsigned int>(mRotationState) + 1);
    nextRotationStateIndex %= static_cast<unsigned int>(eRotationState::End);

    mRotationState = static_cast<eRotationState>(nextRotationStateIndex);
}

void Tetromino::ResetStates()
{
    mMoveOffset = { 0, 0 };
    mRotationState = eRotationState::Degree0;
}

