#pragma once

#include "MainBoard.h"
#include "TetrominoManager.h"
#include "HoldManager.h"
#include "GameStats.h"

class GamePlayStage
{
public:
    GamePlayStage(MainBoard* mainBoard, TetrominoManager* tetrominoManager, GameStats* gameStats);
    virtual ~GamePlayStage() = default;

    void Update(double deltaTime);

private:
    MainBoard* mMainBoard;
    TetrominoManager* mTetrominoManager;
    GameStats* mGameStats;

    // Timer
    double mDropTimer;
    double mTickRate;
};

