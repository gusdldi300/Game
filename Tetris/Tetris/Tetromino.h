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

class MainStage;

class Tetromino
{
public:
    Tetromino(eTetrominoType type);
    virtual ~Tetromino() = default;
    
    std::vector<Position> GetBlockPositions() const;
    bool MoveOneStep(eDirection direction, const MainStage& grid);
    bool MovePosition(Position position, const MainStage& grid);

    bool RotateCW(const MainStage& grid);

    void ResetStates();

protected:
    bool canPlaceOnGrid(Position position, const MainStage& grid) const;

public:
    static const unsigned int TYPES_COUNT;

private:
    static const unsigned int ROTATATION_STATES_COUNT;
    
    static const Position ONE_STEP_MOVE_OFFSETS[];
    static const std::vector<Position> BLOCK_POSITIONS[][4];

protected:
    // Todo: 센터 포지션 하나만 있으면, 나머지 좌표들 모두 계산 가능함. 이렇게 바꾸기
    //std::vector<Position> mBlockPositions;

    Position mMoveOffset;
    eRotationState mRotationState;
    eTetrominoType mType;
};

