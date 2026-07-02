#pragma once

#include <vector>

#include "eDirection.h"
#include "Position.h"

enum class eTetrominoType
{
    I,
    O,
    T,
    L,
    J,
    S,
    Z
};

enum class eRotationState
{
    Degree0,
    Degree90,
    Degree180,
    Degree270,
    End
};

class MainBoard;

class Tetromino
{
public:
    Tetromino(eTetrominoType type);
    virtual ~Tetromino() = default;
    
    Position GetPositionMoveOffset() const;
    std::vector<Position> GetBlockPositions() const;
    std::vector<Position> GetRotatedBlockPositions(eRotationState rotationState) const;

    eRotationState GetRotationState() const;
    eTetrominoType GetType() const;

    void MovePosition(Position position);
    void RotateCW();

    void ResetStates();

protected:

public:
    static const unsigned int TYPES_COUNT;

private:
    static const unsigned int ROTATATION_STATES_COUNT;
    
    static const std::vector<Position> BLOCK_POSITIONS[][4];

protected:
    // Todo: 센터 포지션 하나만 있으면, 나머지 좌표들 모두 계산 가능함. 이렇게 바꾸기
    //std::vector<Position> mBlockPositions;

    Position mMoveOffset;
    eRotationState mRotationState;
    eTetrominoType mType;
};

