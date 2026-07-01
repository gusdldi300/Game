
#include "MainStage.h"
#include "Tetromino.h"

const unsigned int Tetromino::TYPES_COUNT = 7U;
const unsigned int Tetromino::ROTATATION_STATES_COUNT = 4U;

const Position Tetromino::ONE_STEP_MOVE_OFFSETS[] =
{
    { 0, 1 },
    { 1, 0 },
    { 0, -1 }
};

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

bool Tetromino::MoveOneStep(eDirection direction, const MainStage& grid)
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

bool Tetromino::MovePosition(Position position, const MainStage& grid)
{
    if (canPlaceOnGrid(position, grid) == false)
    {
        return false;
    }

    mMoveOffset = position;
    
    return true;
}

std::vector<Position> Tetromino::GetBlockPositions() const
{
    std::vector<Position> blockPositions;
    //blockPositions.reserve(BLOCKS_COUNT);

    unsigned int typeIndex = static_cast<unsigned int>(mType);
    unsigned int rotationIndex = static_cast<unsigned int>(mRotationState);
    for (const Position& position : BLOCK_POSITIONS[typeIndex][rotationIndex])
    {
        blockPositions.push_back(position + mMoveOffset);
    }

    return blockPositions;
}

bool Tetromino::RotateCW(const MainStage& grid)
{
    unsigned int nextRotationStateIndex = (static_cast<unsigned int>(mRotationState) + 1);
    nextRotationStateIndex %= static_cast<unsigned int>(eRotationState::End);

    unsigned int typeIndex = static_cast<unsigned int>(mType);

    for (const Position& position : BLOCK_POSITIONS[typeIndex][nextRotationStateIndex])
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


void Tetromino::ResetStates()
{
    mMoveOffset = { 0, 0 };
    mRotationState = eRotationState::Degree0;
}

bool Tetromino::canPlaceOnGrid(Position position, const MainStage& grid) const
{
    // Todo: 경계가 헷갈림. 상수 수정 필요
    int positionRow = position.GetRow();
    int positionCol = position.GetCol();

    if (positionRow < MainStage::SPAWN_TETROMINO_ROW || positionRow >= MainStage::GRID_ROW_SIZE - 1 ||
        positionCol <= 0 || positionCol >= MainStage::GRID_COL_SIZE - 1)
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