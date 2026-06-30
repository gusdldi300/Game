#pragma once

// Todo: Make as class, add operations() methods
class Position
{
public:
    Position(int row, int col);
    ~Position() = default;

    int GetRow() const;
    int GetCol() const;

    Position operator+(const Position& other) const;
    void operator+=(const Position& other);
    void operator=(const Position& other);

private:
    int mRow;
    int mCol;
};

