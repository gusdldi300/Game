#pragma once

#include <list>

#include "GraphicsObject.h"

class Tetromino;

class TetrominoManager //: public GraphicsObject
{
public:
    TetrominoManager();
    ~TetrominoManager();

    void Release(Tetromino* tetromino);
    Tetromino* GetNextTetromino();

private:
    static const unsigned int MAX_NEXT_TETROMINOS_COUNT;

    Tetromino* mMainTetromino;
    Tetromino* mHoldTetromino;

    std::list<Tetromino*> mNextTetrominoList;

    // Todo: To class GameInfo 
    unsigned int mStageLevel;
    unsigned int mTotalScore;
};

