#pragma once

#include "MainBoard.h"
#include "TetrominoManager.h"
#include "HoldManager.h"
#include "GameStats.h"
#include "GameStage.h"

class GamePlayStage : public GameStage
{
public:
    GamePlayStage(MainBoard* mainBoard, TetrominoManager* tetrominoManager, GameStats* gameStats);
    virtual ~GamePlayStage() = default;

    void Update(double deltaTime);
    void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) override;

private:
    void drawTetromino(Vector2 startVector, const Tetromino& tetromino, HDC memoryDeviceContext);

private:
    MainBoard* mMainBoard;
    TetrominoManager* mTetrominoManager;
    GameStats* mGameStats;

    // Timer
    double mDropTimer;
    double mTickRate;
};

