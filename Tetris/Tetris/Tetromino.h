#pragma once

#include <vector>

#include "eDirection.h"
#include "eColor.h"
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

class Tetromino final
{
public:
    Tetromino(eTetrominoType type);
    virtual ~Tetromino() = default;
    
    eColor GetColor() const;
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
    enum
    {
        TYPES_COUNT = 7U
    };

    static const Position ONE_STEP_MOVE_OFFSETS[];

private:
    enum
    {
        ROTATATION_STATES_COUNT = 4U
    };

    Position mMoveOffset;
    eRotationState mRotationState;
    eTetrominoType mType;

    static const std::vector<Position> BLOCK_POSITIONS[TYPES_COUNT][ROTATATION_STATES_COUNT];
};

