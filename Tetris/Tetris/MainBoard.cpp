
#include <cassert>
#include <string>

#include "MainBoard.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Tetromino.h"

#include "TetrominoManager.h"
#include "HoldManager.h"

const Position MainBoard::ONE_STEP_MOVE_OFFSETS[] =
{
    { 0, 1 },
    { 1, 0 },
    { 0, -1 }
};

MainBoard::MainBoard(Tetromino* tetromino)
    : mActiveTetromino(tetromino) 
    , mHoldTetrominoOrNull(nullptr)
{
    respawnActiveTetromino();
    
    mbBoard = new bool* [BOARD_ROW_SIZE];
    for (int row = 0; row < BOARD_ROW_SIZE; ++row)
    {
        mbBoard[row] = new bool[BOARD_COL_SIZE];

        memset(mbBoard[row], false, sizeof(bool) * BOARD_COL_SIZE);
    }
}

MainBoard::~MainBoard()
{
    for (int row = 0; row < BOARD_ROW_SIZE; ++row)
    {
        delete[] mbBoard[row];
    }

    delete[] mbBoard;
}

const bool* const* MainBoard::GetGrid() const
{
    return mbBoard;
}

unsigned int MainBoard::ClearFullLines()
{
    unsigned int lineClearCount = 0;

    // Todo: Magic number
    for (unsigned int row = SPAWN_ZONE_ROW_SIZE; row < BOARD_ROW_SIZE; ++row)
    {
        bool bLineFull = true;

        for (unsigned int col = 0; col < BOARD_COL_SIZE; ++col)
        {
            if (mbBoard[row][col] == false)
            {
                bLineFull = false;

                break;
            }
        }

        if (bLineFull == false)
        {
            continue;
        }

        ++lineClearCount;

        for (unsigned int copyRow = row; copyRow >= SPAWN_ZONE_ROW_SIZE; --copyRow)
        {
            for (unsigned int copyCol = 0; copyCol < BOARD_COL_SIZE; ++copyCol)
            {
                mbBoard[copyRow][copyCol] = mbBoard[copyRow - 1][copyCol];
            }
        }
    }

    return lineClearCount;
}

void MainBoard::ReleaseActiveTetromino(TetrominoManager* tetrominoManager)
{
    tetrominoManager->Release(mActiveTetromino);

    // Todo: nullptr 을 만들면 안됨
    mActiveTetromino = nullptr;
}

void MainBoard::SetNextTetrominoFrom(TetrominoManager* tetrominoManager)
{
    assert(tetrominoManager != nullptr);

    mActiveTetromino = tetrominoManager->GetNextTetromino();
    respawnActiveTetromino();
}

const Tetromino* MainBoard::GetActiveTetromino() const
{
    return mActiveTetromino;
}

const Tetromino* MainBoard::GetHoldTetrominoOrNull() const
{
    return mHoldTetrominoOrNull;
}


void MainBoard::LockDownTetromino(TetrominoManager* tetrominoManager)
{
    for (const Position& blockPosition : mActiveTetromino->GetBlockPositions())
    {
        mbBoard[blockPosition.GetRow()][blockPosition.GetCol()] = true;
    }
}

bool MainBoard::IsGameOver() const
{
    for (unsigned int col = 0; col < BOARD_COL_SIZE; ++col)
    {
        if (mbBoard[SPAWN_ZONE_ROW_SIZE - 1][col])
        {
            return true;
        }
    }

    return false;
}

bool MainBoard::MoveTetrominoOneStep(eDirection direction)
{
    unsigned int dirIndex = static_cast<unsigned int>(direction);

    for (const Position& blockPosition : mActiveTetromino->GetBlockPositions())
    {
        Position nextPosition = blockPosition + ONE_STEP_MOVE_OFFSETS[dirIndex];
        if (canPlaceOnGrid(nextPosition) == false)
        {
            return false;
        }
    }

    Position newPosition = mActiveTetromino->GetPositionMoveOffset();
    newPosition += MainBoard::ONE_STEP_MOVE_OFFSETS[dirIndex];

    mActiveTetromino->MovePosition(newPosition);

    return true;
}

bool MainBoard::RotateTetrominoCW()
{
    unsigned int nextRotationStateIndex = (static_cast<unsigned int>(mActiveTetromino->GetRotationState()) + 1);
    nextRotationStateIndex %= static_cast<unsigned int>(eRotationState::End);

    eRotationState nextRotationState = static_cast<eRotationState>(nextRotationStateIndex);

    unsigned int typeIndex = static_cast<unsigned int>(mActiveTetromino->GetType());

    for (const Position& rotatedPosition : mActiveTetromino->GetRotatedBlockPositions(nextRotationState))
    {
        if (canPlaceOnGrid(rotatedPosition) == false)
        {
            return false;
        }
    }

    mActiveTetromino->RotateCW();

    return true;
}

bool MainBoard::AddHold(TetrominoManager* tetrominoManager)
{
    if (mbHoldUsed || mHoldTetrominoOrNull != nullptr)
    {
        return false;
    }

    mHoldTetrominoOrNull = mActiveTetromino;
    mHoldTetrominoOrNull->ResetStates();

    SetNextTetrominoFrom(tetrominoManager);

    return true;
}

bool MainBoard::UseHold(TetrominoManager* tetrominoManager)
{
    if (mbHoldUsed || mHoldTetrominoOrNull == nullptr)
    {
        return false;
    }

    ReleaseActiveTetromino(tetrominoManager);

    mActiveTetromino = mHoldTetrominoOrNull;
    respawnActiveTetromino();

    mHoldTetrominoOrNull = nullptr;
    mbHoldUsed = true;

    return true;
}

bool MainBoard::canPlaceOnGrid(Position position) const
{
    // Todo: 경계가 헷갈림. 상수 수정 필요
    int positionRow = position.GetRow();
    int positionCol = position.GetCol();

    if (positionRow < 0 || positionRow >= BOARD_ROW_SIZE ||
        positionCol < 0 || positionCol >= BOARD_COL_SIZE)
    {
        return false;
    }

    if (mbBoard[positionRow][positionCol])
    {
        return false;
    }

    return true;
}

// Todo: 이름 변경
void MainBoard::respawnActiveTetromino()
{
    assert(mActiveTetromino != nullptr);

    // Todo: Magic number
    mActiveTetromino->ResetStates();
    mActiveTetromino->MovePosition({ SPAWN_TETROMINO_ROW, SPAWN_TETROMINO_COL });
}

