#pragma once

#include <list>

#include "GraphicsGrid.h"

class Tetromino;

class TetrominoManager : public GraphicsGrid
{
public:
    TetrominoManager(Vector2 leftTopPosition);
    virtual ~TetrominoManager();

    void Update();
    //void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution);

    void Release(Tetromino* tetromino);
    Tetromino* GetNextTetromino();

private:
    static const unsigned int GRID_ROW_SIZE;
    static const unsigned int GRID_COL_SIZE;

    static const unsigned int MAX_NEXT_TETROMINOS_COUNT;
    static const unsigned int MAX_NEXT_TETROMINOS_SHOW_COUNT;

    std::list<Tetromino*> mNextTetrominoList;
};

