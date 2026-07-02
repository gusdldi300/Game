#pragma once

#include <wtypes.h>
#include <list>

#include "GraphicsGrid.h"
#include "Tetromino.h"

// Todo: #include, 전방선언 정리하기
class Position;
class TetrominoManager;
class HoldManager;

class MainBoard : public GraphicsGrid
{
public:
    MainBoard(Vector2 leftTopVector, Tetromino* tetromino);
    virtual ~MainBoard() = default;

    // Todo: Move to private
    bool MoveTetrominoOneStep(eDirection direction);
    bool RotateTetrominoCW();

    unsigned int ClearLines(TetrominoManager* tetrominoManager);

    void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) override;
    
    const bool* const* GetGrid() const;
    
    // Todo: Check later
    const Tetromino* GetTetromino() const;
    const void SetTetromino(Tetromino* tetromino);

private:
    bool canPlaceOnGrid(Position position) const;
    void spawnTetromino();

public:
    // Todo: 쓸모없는 정적 변수 너무 많음
    static const unsigned int SPAWN_ZONE_ROW_SIZE = 2U;

    static const unsigned int GRID_ROW_SIZE = 20U + SPAWN_ZONE_ROW_SIZE + WALL_ROW_SIZE;
    static const unsigned int GRID_COL_SIZE = 10U + WALL_COL_SIZE;

    static const unsigned int MAX_NEXT_TETROMINOS_COUNT = 5U;
    static const unsigned int MAX_MOVE_POSITIONS_COUNT = 4U;

    static const unsigned int SPAWN_TETROMINO_ROW = 1U;
    static const unsigned int SPAWN_TETROMINO_COL = 3U;

private:
    static const Vector2 GRID_START_POINT;
    static const Position ONE_STEP_MOVE_OFFSETS[];

    Tetromino* mTetromino;
    
    Tetromino* mHoldTetrominoOrNull;
    bool mbHoldUsed;

    // Todo: To class GameInfo
    //unsigned int mTotalScore;
    //unsigned int mStageLevel;
};


