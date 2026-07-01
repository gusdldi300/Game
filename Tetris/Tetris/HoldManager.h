#pragma once

#include "GraphicsGrid.h"

class Tetromino;

class HoldManager
{
public:
    HoldManager(Vector2 leftTopPosition);
    virtual ~HoldManager() = default;

    bool CanUseHold() const;
    bool HasHold() const;

    bool setHold(Tetromino* tetromino);

public:
    static const unsigned int GRID_ROW_SIZE;
    static const unsigned int GRID_COL_SIZE;

private:
    bool mbUsedStage;
    Tetromino* mTetrominoOrNull;
};

