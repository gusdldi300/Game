
#include "Tetromino.h"
#include "MainBoard.h"

const Position Tetromino::ONE_STEP_MOVE_OFFSETS[] =
{
    { 0, 0 },
    { 0, 1 },
    { 1, 0 },
    { 0, -1 }
};

// Todo: Check s tetromino, akward rotation
const std::vector<Position> Tetromino::BLOCK_POSITIONS[TYPES_COUNT][ROTATATION_STATES_COUNT] =
{
    // 1. Tetromino I
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } }, // Degree 0
        { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 } },
        { { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } }
    },

    // 2. Tetromino O
    {
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
    },

    // 3. Tetromino T
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 1 } },
        { { 0, 2 }, { 1, 1 }, { 1, 2 }, { 2, 2 } }, 
        { { 1, 1 }, { 2, 0 }, { 2, 1 }, { 2, 2 } },
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 0 } } 
    },

    // 4. Tetromino L
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 0 } },
        { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 } },
        { { 0, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 } },
        { { 1, 2 }, { 2, 0 }, { 2, 1 }, { 2, 2 } }
    },

    // 5. Tetromino J
    {
        { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 2 } },
        { { 0, 2 }, { 1, 2 }, { 2, 1 }, { 2, 2 } },
        { { 1, 0 }, { 2, 0 }, { 2, 1 }, { 2, 2 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 2, 0 } }
    },

    // 6. Tetromino S
    {
        { { 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 1 } }, 
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
        { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 1, 1 }, { 1, 2 }, { 2, 0 }, { 2, 1 } }
    },

    // 7. Tetromino Z
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

eColor Tetromino::GetColor() const
{
    return static_cast<eColor>(mType);
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

