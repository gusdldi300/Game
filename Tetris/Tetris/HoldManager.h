#pragma once

#include "GameStage.h"

class Tetromino;
class TetrominoManager;
class MainBoard;

class HoldManager
{
public:
    HoldManager(Vector2 leftTopPosition);
    virtual ~HoldManager() = default;

    bool HasUsedHold() const;
    bool HasHold() const;

    bool SetHold(Tetromino* holdTetromino);
    Tetromino* UseHoldOrNull();

    bool UseHold(MainBoard* mainBoard, TetrominoManager* terominoManager);

private:
    bool mbUsedHoldStage;
    Tetromino* mHoldTetrominoOrNull;
};

