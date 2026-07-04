
#include "MainBoard.h"
#include "Tetromino.h"

const unsigned int Tetromino::TYPES_COUNT = 7U;

const unsigned int DIRECTION_NONE = 10U;
const unsigned int Tetromino::ROTATATION_STATES_COUNT = 4U;

const Position Tetromino::ONE_STEP_MOVE_OFFSETS[] =
{
    { 0, 0 },
    { 0, 1 },
    { 1, 0 },
    { 0, -1 }
};

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

std::vector<Position> Tetromino::GetBlockPositions() const
{
    return getTransformedBlockPositions(mRotationState, eDirection::None);
}

std::vector<Position> Tetromino::GetRotatedCWBlockPositions() const
{
    eRotationState nextRotationState = static_cast<eRotationState>(static_cast<unsigned int>(mRotationState) + 1);

    return getTransformedBlockPositions(nextRotationState, eDirection::None);
}

std::vector<Position> Tetromino::GetMovedOneStepPositions(eDirection direction) const
{
    return getTransformedBlockPositions(mRotationState, direction);
}

std::vector<Position> Tetromino::getTransformedBlockPositions(eRotationState rotationState, eDirection direction) const
{
    std::vector<Position> blockPositions;
    //blockPositions.reserve(BLOCKS_COUNT);

    unsigned int typeIndex = static_cast<unsigned int>(mType);
    unsigned int rotationIndex = static_cast<unsigned int>(rotationState);
    unsigned int directionIndex = static_cast<unsigned int>(direction);

    Position moveOffset = mMoveOffset + ONE_STEP_MOVE_OFFSETS[directionIndex];

    for (const Position& position : BLOCK_POSITIONS[typeIndex][rotationIndex])
    {
        blockPositions.push_back(position + moveOffset);
    }

    return blockPositions;
}

eRotationState Tetromino::GetRotationState() const
{
    return mRotationState;
}

void Tetromino::MoveOneStep(eDirection direction)
{
    mMoveOffset += ONE_STEP_MOVE_OFFSETS[static_cast<unsigned int>(direction)];
}

void Tetromino::RotateCW()
{
    unsigned int nextRotationStateIndex = (static_cast<unsigned int>(mRotationState) + 1);
    nextRotationStateIndex %= static_cast<unsigned int>(eRotationState::End);

    mRotationState = static_cast<eRotationState>(nextRotationStateIndex);
}

void Tetromino::SetMoveOffset(Position moveOffset)
{
    mMoveOffset = moveOffset;
}

void Tetromino::ResetStates()
{
    mMoveOffset = { 0, 0 };
    mRotationState = eRotationState::Degree0;
}

