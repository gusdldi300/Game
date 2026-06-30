
#include "Tetromino.h"
#include "Grid.h"

const std::vector<Position>& Tetromino::GetBlockPositions() const
{
    return mBlockPositions;
}

void Tetromino::MoveOneStep(eDirection direction)
{
    unsigned int dirIndex = static_cast<unsigned int>(direction);

    for (unsigned int i = 0; i < mBlockPositions.size(); ++i)
    {
        mBlockPositions[i].Row += Grid::MOVE_POSITIONS[dirIndex].Row;
        mBlockPositions[i].Col += Grid::MOVE_POSITIONS[dirIndex].Col;
    }
}

// Todo: Change to MovePositionOnGrid()
void Tetromino::MovePosition(Position position)
{
    // Todo: position assert

    for (Position& blockPosition : mBlockPositions)
    {
        blockPosition.Row += position.Row;
        blockPosition.Col += position.Col;
    }
}

void Tetromino::RotateCW()
{
    std::vector<Position> rotatedPositions;

    // Use wall kick
    for (int i = 0; i < mBlockPositions.size(); ++i)
    {
        int nextRow = mBlockPositions[i].Col * -1;
        int nextCol = mBlockPositions[i].Row;

        rotatedPositions.push_back({ nextRow, nextCol });
    }

    int minRow = 0;
    int minCol = 0;

    for (int i = 0; i < mBlockPositions.size(); ++i)
    {
        if (rotatedPositions[i].Row < minRow)
        {
            minRow = rotatedPositions[i].Row;
        }

        if (rotatedPositions[i].Col < minCol)
        {
            minCol = rotatedPositions[i].Col;
        }
    }

    if (minRow < 0) 
    {
        int offsetRow = minRow * -1; 
        for (int i = 0; i < mBlockPositions.size(); ++i)
        {
            mBlockPositions[i].Row = rotatedPositions[i].Row + offsetRow;
        }
    }

    if (minCol < 0) 
    {
        int offsetCol = minCol * -1;
        for (int i = 0; i < mBlockPositions.size(); ++i)
        {
            mBlockPositions[i].Col = rotatedPositions[i].Col + offsetCol;
        }
    }

}
