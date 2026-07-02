
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

MainBoard::MainBoard(Vector2 leftTopVector, Tetromino* tetromino)
    : GraphicsGrid(leftTopVector, GRID_ROW_SIZE, GRID_COL_SIZE)
    , mTetromino(tetromino) // Todo: mTetromino to nullptr
{
    // Todo: 시간 결합 문제
    spawnTetromino();
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
                    spawnTetromino();
                }
                else
                {
                    mTetromino = mHoldTetrominoOrNull;
                    assert(mTetromino != nullptr);

                    mHoldTetrominoOrNull = nullptr;
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

// Todo: HDC 인자 중 하나는 전달할 필요 없음
void MainBoard::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    GraphicsGrid::Render(windowDeviceContext, memoryDeviceContext, windowResolution);

    // Todo: Code duplicate
    // Draw tetromino
    {
        for (const Position& blockPosition : mTetromino->GetBlockPositions())
        {
            int renderStartY = (blockPosition.GetRow() * BLOCK_LENGTH);
            int renderStartX = (blockPosition.GetCol() * BLOCK_LENGTH);

            Rectangle(memoryDeviceContext,
                renderStartX,
                renderStartY,
                renderStartX + BLOCK_LENGTH,
                renderStartY + BLOCK_LENGTH);
        }
    }

    // Draw strings
    /*
    {
        std::wstring printScore = L"Score: " + std::to_wstring(mTotalScore);
        std::wstring printLevel = L"Stage Level: " + std::to_wstring(mStageLevel);

        // Todo: No magic number, move position
        const float GRID_HEIGHT = mGridRowSize * BLOCK_LENGTH;
        const float PRINT_OFFSET = 10.f;
        const float PRINT_STRING_OFFSET = 30.f;
        
        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET, printScore.c_str(), printScore.length());
        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET + PRINT_STRING_OFFSET, printLevel.c_str(), printLevel.length());
    }
    */
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

