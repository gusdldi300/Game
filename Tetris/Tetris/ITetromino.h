#pragma once

#include "Tetromino.h"

class ITetromino : public Tetromino
{
public:
    ITetromino();
    virtual ~ITetromino() = default;

private:
    enum
    {
        BLOCKS_COUNT = 4U
    };
};

