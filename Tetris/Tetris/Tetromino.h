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
    
    std::vector<Position> GetBlockPositions() const;

    std::vector<Position> GetRotatedCWBlockPositions() const;
    std::vector<Position> GetMovedOneStepPositions(eDirection direction) const;
    
    eRotationState GetRotationState() const;

    //void SetMoveOffset(Position position);
    void MoveOneStep(eDirection direction);
    void RotateCW();
    void SetMoveOffset(Position moveOffset);

    void ResetStates();

private:
    std::vector<Position> getTransformedBlockPositions(eRotationState rotationState, eDirection direction) const;

public:
    static const unsigned int TYPES_COUNT;
    static const Position ONE_STEP_MOVE_OFFSETS[];

protected:
    Position mMoveOffset;
    eRotationState mRotationState;
    eTetrominoType mType;

private:
    static const unsigned int ROTATATION_STATES_COUNT;
    static const unsigned int DIRECTION_NONE;

    // Todo: magic number
    static const std::vector<Position> BLOCK_POSITIONS[][4];
};

