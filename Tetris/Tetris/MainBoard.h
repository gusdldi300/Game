#pragma once

#include <wtypes.h>
#include <list>

#include "GameStage.h"
#include "Tetromino.h"

// Todo: #include, 전방선언 정리하기
class Position;
class TetrominoManager;
class HoldManager;

class MainBoard
{
public:
    MainBoard(Tetromino* tetromino);
    virtual ~MainBoard();

    // Todo: Move to private
    bool MoveTetrominoOneStep(eDirection direction);
    bool RotateTetrominoCW();

    bool AddHold(TetrominoManager* tetrominoManager);
    bool UseHold(TetrominoManager* tetrominoManager);

    unsigned int ClearFullLines();
    void LockDownTetromino(TetrominoManager* tetrominoManager);
    bool IsGameOver() const;

    const bool* const* GetGrid() const;

    // Todo: Check later
    const Tetromino* GetActiveTetromino() const;
    const Tetromino* GetHoldTetrominoOrNull() const;

    // Todo: 사용하기 헷갈림
    void ReleaseActiveTetromino(TetrominoManager* tetrominoManager);
    void SetNextTetrominoFrom(TetrominoManager* tetrominoManager);

private:
    bool canPlaceOnGrid(Position position) const;
    void respawnActiveTetromino();

public:
    // Todo: 쓸모없는 정적 변수 너무 많음
    // cpp 에 초기화하기
    static const unsigned int WALL_COL_SIZE = 2U;
    static const unsigned int WALL_ROW_SIZE = 2U;

    static const unsigned int SPAWN_ZONE_ROW_SIZE = 2U;
    
    static const unsigned int GRID_ROW_SIZE = 20U + SPAWN_ZONE_ROW_SIZE + WALL_ROW_SIZE;
    static const unsigned int GRID_COL_SIZE = 10U + WALL_COL_SIZE;

private:
    static const unsigned int BOARD_START_ROW = 1U;
    static const unsigned int BOARD_START_COL = 1U;
    static const unsigned int BOARD_END_COL = GRID_COL_SIZE - 2;
    static const unsigned int BOARD_END_ROW = GRID_ROW_SIZE - 2;
    
    static const unsigned int MAX_NEXT_TETROMINOS_COUNT = 5U;
    static const unsigned int MAX_MOVE_POSITIONS_COUNT = 4U;

    static const unsigned int SPAWN_TETROMINO_ROW = 1U;
    static const unsigned int SPAWN_TETROMINO_COL = 3U;

    static const Vector2 GRID_START_POINT;
    static const Position ONE_STEP_MOVE_OFFSETS[];

    Tetromino* mActiveTetromino;
    
    Tetromino* mHoldTetrominoOrNull;
    bool mbHoldUsed;

    bool** mbGrid;
};


