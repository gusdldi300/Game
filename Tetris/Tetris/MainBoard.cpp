
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
    : mTetromino(tetromino) 
    , mHoldTetrominoOrNull(nullptr)
{
    // Todo: 시간 결합 문제
    spawnTetromino();

    mbGrid = new bool* [GRID_ROW_SIZE];
    for (int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        mbGrid[row] = new bool[GRID_COL_SIZE];

        memset(mbGrid[row], false, sizeof(bool) * GRID_COL_SIZE);
    }

    // Todo: row = SPAWN_ZONE_ROW_SIZE
    for (unsigned int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        mbGrid[row][0] = true;
        mbGrid[row][GRID_COL_SIZE - 1] = true;
    }

    for (unsigned int col = 0; col < GRID_COL_SIZE; ++col)
    {
        mbGrid[0][col] = true;
        mbGrid[GRID_ROW_SIZE - 1][col] = true;
    }
}

MainBoard::~MainBoard()
{
    for (int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        delete[] mbGrid[row];
    }

    delete[] mbGrid;
}

const bool* const* MainBoard::GetGrid() const
{
    return mbGrid;
}

unsigned int MainBoard::ClearLines(TetrominoManager* tetrominoManager)
{
    bool bTetrominoAlive = true;
    // Move tetromino
    {
        if (TimeManager::GetInstance()->HasTicked())
        {
            TimeManager::GetInstance()->ResetTick();

            bTetrominoAlive = MoveTetrominoOneStep(eDirection::Down);
        }

        // Todo: Maybe change to switch case
        KeyManager* keyManager = KeyManager::GetInstance();
        if (keyManager->GetKeyState(eKey::Left) == eKeyState::Press)
        {
            MoveTetrominoOneStep(eDirection::Left);
        }
        else if (keyManager->GetKeyState(eKey::Right) == eKeyState::Press)
        {
            MoveTetrominoOneStep(eDirection::Right);
        }
        else if (keyManager->GetKeyState(eKey::Up) == eKeyState::Press)
        {
            RotateTetrominoCW();
        }
        else if (keyManager->GetKeyState(eKey::Space) == eKeyState::Press)
        {
            while (bTetrominoAlive)
            {
                bTetrominoAlive = MoveTetrominoOneStep(eDirection::Down);
            }

            assert(bTetrominoAlive == false);
        }
        else if (keyManager->GetKeyState(eKey::A) == eKeyState::Press)
        {
            // Todo: Maybe use MainBoard.UseHold
            // Use hold
            if (mbHoldUsed == false)
            {
                //assert(mTetromino != nullptr);

                if (mHoldTetrominoOrNull == nullptr)
                {
                    mHoldTetrominoOrNull = mTetromino;
                    mHoldTetrominoOrNull->ResetStates();

                    mTetromino = tetrominoManager->GetNextTetromino();
                    // Todo: spawn() 헷갈림
                    spawnTetromino();
                }
                else
                {
                    mTetromino = mHoldTetrominoOrNull;
                    assert(mTetromino != nullptr);
                    spawnTetromino();

                    mHoldTetrominoOrNull = nullptr;
                    mbHoldUsed = true;
                }
            }
        }
    }

    if (bTetrominoAlive)
    {
        return 0;
    }

    // Mark dead tetromino on grid, and respawn tetromino
    {
        for (const Position& blockPosition : mTetromino->GetBlockPositions())
        {
            mbGrid[blockPosition.GetRow()][blockPosition.GetCol()] = true;
        }

        tetrominoManager->Release(mTetromino);

        mTetromino = tetrominoManager->GetNextTetromino();
        spawnTetromino();
    }

    //  Clear full lines 
    unsigned int lineClearCount = 0;
    {
        // Todo: Magic number
        for (unsigned int row = SPAWN_ZONE_ROW_SIZE; row < GRID_ROW_SIZE - 1; ++row)
        {
            bool bLineFull = true;

            for (unsigned int col = 1; col < GRID_COL_SIZE - 1; ++col)
            {
                if (mbGrid[row][col] == false)
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
                for (unsigned int copyCol = 1; copyCol < GRID_COL_SIZE - 1; ++copyCol)
                {
                    mbGrid[copyRow][copyCol] = mbGrid[copyRow - 1][copyCol];
                }
            }
        }
    }

    return lineClearCount;
}

const Tetromino* MainBoard::GetTetromino() const
{
    return mTetromino;
}

const Tetromino* MainBoard::GetHoldTetrominoOrNull() const
{
    return mHoldTetrominoOrNull;
}

const void MainBoard::SetTetromino(Tetromino* tetromino)
{
    mTetromino = tetromino;
}

bool MainBoard::MoveTetrominoOneStep(eDirection direction)
{
    unsigned int dirIndex = static_cast<unsigned int>(direction);

    for (const Position& blockPosition : mTetromino->GetBlockPositions())
    {
        Position nextPosition = blockPosition + ONE_STEP_MOVE_OFFSETS[dirIndex];
        if (canPlaceOnGrid(nextPosition) == false)
        {
            return false;
        }
    }

    Position newPosition = mTetromino->GetPositionMoveOffset();
    newPosition += MainBoard::ONE_STEP_MOVE_OFFSETS[dirIndex];

    mTetromino->MovePosition(newPosition);

    return true;
}

bool MainBoard::RotateTetrominoCW()
{
    unsigned int nextRotationStateIndex = (static_cast<unsigned int>(mTetromino->GetRotationState()) + 1);
    nextRotationStateIndex %= static_cast<unsigned int>(eRotationState::End);

    eRotationState nextRotationState = static_cast<eRotationState>(nextRotationStateIndex);

    unsigned int typeIndex = static_cast<unsigned int>(mTetromino->GetType());

    for (const Position& rotatedPosition : mTetromino->GetRotatedBlockPositions(nextRotationState))
    {
        if (canPlaceOnGrid(rotatedPosition) == false)
        {
            return false;
        }
    }

    mTetromino->RotateCW();

    return true;
}

bool MainBoard::canPlaceOnGrid(Position position) const
{
    // Todo: 경계가 헷갈림. 상수 수정 필요
    int positionRow = position.GetRow();
    int positionCol = position.GetCol();

    if (positionRow < MainBoard::SPAWN_TETROMINO_ROW || positionRow >= MainBoard::GRID_ROW_SIZE - 1 ||
        positionCol <= 0 || positionCol >= MainBoard::GRID_COL_SIZE - 1)
    {
        return false;
    }

    if (mbGrid[positionRow][positionCol])
    {
        return false;
    }

    return true;
}

// Todo: 이름 변경
void MainBoard::spawnTetromino()
{
    assert(mTetromino != nullptr);

    // Todo: Magic number
    mTetromino->ResetStates();
    mTetromino->MovePosition({ SPAWN_TETROMINO_ROW, SPAWN_TETROMINO_COL });
}

