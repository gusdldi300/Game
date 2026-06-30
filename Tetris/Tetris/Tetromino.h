#pragma once

#include <vector>

#include "eDirection.h"
#include "Position.h"

enum class eRotationState
{
    Degree0,
    Degree90,
    Degree180,
    Degree270,
    End
};

class Grid;

class Tetromino
{
public:
    Tetromino(Position moveOffset);
    virtual ~Tetromino() = default;
    
    virtual std::vector<Position> GetBlockPositions() const = 0;
    bool MoveOneStep(eDirection direction, const Grid& grid);
    void MovePosition(Position position);

    virtual bool RotateCW(const Grid& grid) = 0;

    void ResetStates();

protected:
    bool canPlaceOnGrid(Position position, const Grid& grid) const;

private:
    static const Position ONE_STEP_MOVE_OFFSETS[];

protected:
    // Todo: 센터 포지션 하나만 있으면, 나머지 좌표들 모두 계산 가능함. 이렇게 바꾸기
    //std::vector<Position> mBlockPositions;

    Position mMoveOffset;
    eRotationState mRotationState;
};

