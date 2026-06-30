#pragma once

#include <vector>

#include "eDirection.h"
#include "Position.h"

enum class eRotationState
{
    Degree0,
    Degree90,
    Degree180,
    Degree270
};

class Grid;

class Tetromino
{
public:
    Tetromino() = default;
    virtual ~Tetromino() = default;
    
    const std::vector<Position>& GetBlockPositions() const;
    void MoveOneStep(eDirection direction);
    void MovePosition(Position position);
    void RotateCW();

protected:
    // Todo: 센터 포지션 하나만 있으면, 나머지 좌표들 모두 계산 가능함. 이렇게 바꾸기
    std::vector<Position> mBlockPositions;

    //Position mMoveOffset;
    //eRotationState mRotationState;
};

