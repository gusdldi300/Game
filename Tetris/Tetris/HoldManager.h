#pragma once

#include "GraphicsGrid.h"

class Tetromino;

class HoldManager : public GraphicsGrid
{
public:
    HoldManager(Vector2 leftTopPosition);
    virtual ~HoldManager() = default;

    bool HasUsedHold() const;
    bool HasHold() const;

    bool SetHold(Tetromino* holdTetromino);
    Tetromino* UseHoldOrNull();

public:
    static const unsigned int GRID_ROW_SIZE;
    static const unsigned int GRID_COL_SIZE;

private:
    bool mbUsedHoldStage;
    Tetromino* mHoldTetrominoOrNull;
};

