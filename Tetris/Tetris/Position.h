#pragma once

class Position
{
public:
    Position(int row, int col);
    Position(float row, float col);

    ~Position() = default;

    int GetRow() const;
    int GetCol() const;

    Position operator+(const Position& other) const;

    void operator+=(const Position& other);
    void operator-=(const Position& other);
    void operator=(const Position& other);

private:
    int mRow;
    int mCol;
};

