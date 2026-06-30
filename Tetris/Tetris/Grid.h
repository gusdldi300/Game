#pragma once

#include <wtypes.h>
#include <list>

#include "GraphicsObject.h"
#include "Tetromino.h"

class Position;

class Grid : public GraphicsObject
{
public:
    Grid();
    virtual ~Grid();

    void Update() override;
    void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) override;
    
    const bool* const* GetGrid() const;

private:
    void spawnTetromino(Tetromino* tetromino);
    void addRandomTetromino(std::list<Tetromino*>* outNextTetrominoList);

public:
    // Todo: 수정 필요, 동적할당하기
    static const unsigned int WALL_COL_SIZE = 2U;
    static const unsigned int WALL_ROW_SIZE = 1U;
    static const unsigned int SPAWN_ZONE_ROW_SIZE = 2U;

    static const unsigned int GRID_ROW_SIZE = 20U + SPAWN_ZONE_ROW_SIZE + WALL_ROW_SIZE;
    static const unsigned int GRID_COL_SIZE = 10U + WALL_COL_SIZE;

    static const unsigned int MAX_NEXT_TETROMINOS_COUNT = 5U;
    static const unsigned int MAX_MOVE_POSITIONS_COUNT = 4U;

private:
    static const unsigned int SPAWN_TETROMINO_ROW = 0U;
    static const unsigned int SPAWN_TETROMINO_COL = 3U;

    static const Vector2 GRID_START_POINT;
    static const float BLOCK_LENGTH;
    static const float GRID_WIDTH;
    static const float GRID_HEIGHT;

    bool** mbGrid;
    //bool mbGrid[GRID_ROW_SIZE][GRID_COL_SIZE];
    
    std::list<Tetromino*> mNextTetrominoList;
    // Todo: Change to mTetrominoOrNull
    Tetromino* mTetromino;
};


