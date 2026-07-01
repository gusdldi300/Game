#pragma once

#include <wtypes.h>
#include <list>

#include "GraphicsGrid.h"
#include "Tetromino.h"

// Todo: #include, 전방선언 정리하기
class Position;
class TetrominoManager;
class HoldManager;

class MainStage : public GraphicsGrid
{
public:
    MainStage(Vector2 leftTopVector, Tetromino* tetromino);
    virtual ~MainStage();

    //void Update(TetrominoManager* tetrominoManager);
    void Update(TetrominoManager* tetrominoManager);
    void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) override;
    
    const bool* const* GetGrid() const;

private:
    void spawnTetromino();

public:
    // Todo: 수정 필요, 동적할당하기
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
    //static const float BLOCK_LENGTH;
    //static const float GRID_WIDTH;
    //static const float GRID_HEIGHT;

    static const unsigned int STAGES_COUNT;
    static const unsigned int STAGE_LEVEL_REACH_SCORES[];
    static const unsigned int MAX_STAGE_LEVEL;

    //bool** mbGrid;
    //bool mbGrid[GRID_ROW_SIZE][GRID_COL_SIZE];
    
    Tetromino* mTetromino;
    Tetromino* mHoldTetrominoOrNull;
    bool mbUsedHold;

    // Todo: Move to Info class
    unsigned int mTotalScore;
    unsigned int mStageLevel;
};


