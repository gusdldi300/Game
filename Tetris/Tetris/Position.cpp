#include "Position.h"

#include <cassert>

Position::Position(int row, int col)
    : mRow(row)
    , mCol(col)
{
}

int Position::GetRow() const
{
    return mRow;
}

int Position::GetCol() const
{
    return mCol;
}

Position Position::operator+(const Position& other) const
{
    Position sumPosition(mRow, mCol);
    sumPosition += other;

    return sumPosition;
}

void Position::operator+=(const Position& other)
{
    mRow += other.mRow;
    mCol += other.mCol;
}

void Position::operator-=(const Position& other)
{
    mRow -= other.mRow;
    mCol -= other.mCol;
}

void Position::operator=(const Position& other)
{
    assert(&other != this);

    mRow = other.mRow;
    mCol = other.mCol;
}
