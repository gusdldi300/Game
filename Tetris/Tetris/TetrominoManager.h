#pragma once

#include <list>

#include "GameStage.h"

class Tetromino;

class TetrominoManager
{
public:
    TetrominoManager(Vector2 leftTopPosition);
    virtual ~TetrominoManager();

    void Update();

    const std::list<Tetromino*> GetNextTetrominoList() const;
    Tetromino* GetNextTetromino();
    void Release(Tetromino* tetromino);

private:
    static const unsigned int MAX_NEXT_TETROMINOS_COUNT;
    static const unsigned int MAX_NEXT_TETROMINOS_SHOW_COUNT;

    std::list<Tetromino*> mNextTetrominoList;
};

