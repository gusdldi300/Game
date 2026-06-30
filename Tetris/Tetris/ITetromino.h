#pragma once

#include "Tetromino.h"

class ITetromino : public Tetromino
{
public:
    ITetromino(Position moveOffset);
    virtual ~ITetromino() = default;

    std::vector<Position> GetBlockPositions() const;
    bool RotateCW(const Grid& grid) override;

private:
    static const unsigned int BLOCKS_COUNT;
    static const std::vector<Position> BLOCK_POSITIONS[];
};

