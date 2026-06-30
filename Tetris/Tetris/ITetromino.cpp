
#include "ITetromino.h"

ITetromino::ITetromino()
    : Tetromino()
{
    mBlockPositions.push_back({ 0, 0 });
    mBlockPositions.push_back({ 0, 1 });
    mBlockPositions.push_back({ 0, 2 });
    mBlockPositions.push_back({ 0, 3 });
}

