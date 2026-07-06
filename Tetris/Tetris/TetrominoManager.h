#pragma once

#include <list>

#include "GameStage.h"
#include "Tetromino.h"

class Tetromino;

class TetrominoManager
{
public:
    TetrominoManager();
    virtual ~TetrominoManager();

    bool HasUsedHold() const;
    bool HasHoldTetromino() const;
    void SetHoldTetromino(Tetromino* tetromino);
    void ResetHold();

    void Reset();

    const std::list<Tetromino*> GetNextTetrominoList() const;
    
    const Tetromino* GetHoldTetrominoOrNull() const;

    Tetromino* ProvideHoldTetromino();
    Tetromino* ProvideNextTetromino();
    void Release(Tetromino* tetromino);

private:
    enum
    {
        MAX_NEXT_TETROMINOS_COUNT = 40U
    };

    std::list<Tetromino*> mNextTetrominoList;

    Tetromino* mHoldTetrominoOrNull;
    bool mbHoldUsed;
};

