
#include <cassert>
#include <string>

#include "MainBoard.h"
#include "KeyManager.h"
#include "Tetromino.h"

#include "TetrominoManager.h"

MainBoard::MainBoard(Tetromino* tetromino)
    : mActiveTetromino(tetromino) 
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

bool MainBoard::RiseBlocksOneStep()
{
    for (unsigned int copyRow = SPAWN_ZONE_ROW_SIZE; copyRow < (BOARD_ROW_SIZE - 1); ++copyRow)
    {
        for (unsigned int copyCol = 0; copyCol < BOARD_COL_SIZE; ++copyCol)
        {
            mbBoard[copyRow][copyCol] = mbBoard[copyRow + 1][copyCol];
        }
    }

    for (unsigned int col = 0; col < BOARD_COL_SIZE; ++col)
    {
        mbBoard[BOARD_ROW_SIZE - 1][col] = true;
    }

    mbBoard[BOARD_ROW_SIZE - 1][SPAWN_TETROMINO_COL] = false;

    return IsGameOver();
}

unsigned int MainBoard::ClearFullLines()
{
    unsigned int lineClearCount = 0;

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

void MainBoard::ReleaseAndSetNextActiveTetrominoFrom(TetrominoManager* tetrominoManager)
{
    assert(tetrominoManager != nullptr);
    assert(mActiveTetromino != nullptr);

    tetrominoManager->Release(mActiveTetromino);
    mActiveTetromino = tetrominoManager->ProvideNextTetromino();

    respawnActiveTetromino();
}

const Tetromino* MainBoard::GetActiveTetromino() const
{
    return mActiveTetromino;
}

const std::vector<Position> MainBoard::GetGhostTetrominoBlockPositions() const
{
    std::vector<Position> blockPositions = mActiveTetromino->GetBlockPositions();

    while (canPlaceBlocksOnBoard(blockPositions))
    {
        for (Position& blockPosition : blockPositions)
        {
            blockPosition += Tetromino::ONE_STEP_MOVE_OFFSETS[static_cast<unsigned int>(eDirection::Down)];
        }
    }
    
    // Todo: Code duplicated
    for (Position& blockPosition : blockPositions)
    {
        blockPosition -= Tetromino::ONE_STEP_MOVE_OFFSETS[static_cast<unsigned int>(eDirection::Down)];
    }

    return blockPositions;
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

void MainBoard::Reset()
{
    for (unsigned int row = 0; row < BOARD_ROW_SIZE; ++row)
    {
        for (unsigned int col = 0; col < BOARD_COL_SIZE; ++col)
        {
            mbBoard[row][col] = false;
        }
    }
}

bool MainBoard::MoveTetrominoOneStep(eDirection direction)
{
    if (canPlaceBlocksOnBoard(mActiveTetromino->GetMovedOneStepPositions(direction)) == false)
    {
        return false;
    }

    mActiveTetromino->MoveOneStep(direction);

    return true;
}

bool MainBoard::RotateTetrominoCW()
{
    if (canPlaceBlocksOnBoard(mActiveTetromino->GetRotatedCWBlockPositions()) == false)
    {
        return false;
    }

    mActiveTetromino->RotateCW();

    return true;
}

bool MainBoard::AddHold(TetrominoManager* tetrominoManager)
{
    if (tetrominoManager->HasUsedHold() || tetrominoManager->HasHoldTetromino())
    {
        return false;
    }

    tetrominoManager->SetHoldTetromino(mActiveTetromino);
    ReleaseAndSetNextActiveTetrominoFrom(tetrominoManager);

    return true;
}

bool MainBoard::UseHold(TetrominoManager* tetrominoManager)
{
    if (tetrominoManager->HasUsedHold() || tetrominoManager->HasHoldTetromino() == false)
    {
        return false;
    }

    tetrominoManager->Release(mActiveTetromino);
    mActiveTetromino = tetrominoManager->ProvideHoldTetromino();

    respawnActiveTetromino();

    return true;
}

bool MainBoard::canPlaceBlocksOnBoard(const std::vector<Position>& blockPositions) const
{
    for (const Position& blockPosition : blockPositions)
    {
        int positionRow = blockPosition.GetRow();
        int positionCol = blockPosition.GetCol();

        if (positionRow < 0 || positionRow >= BOARD_ROW_SIZE ||
            positionCol < 0 || positionCol >= BOARD_COL_SIZE)
        {
            return false;
        }

        if (mbBoard[positionRow][positionCol])
        {
            return false;
        }
    }

    return true;
}

void MainBoard::respawnActiveTetromino()
{
    assert(mActiveTetromino != nullptr);

    mActiveTetromino->ResetStates();
    mActiveTetromino->SetMoveOffset({ SPAWN_TETROMINO_ROW, SPAWN_TETROMINO_COL });
}

