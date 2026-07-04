#pragma once

#include "MainBoard.h"
#include "TetrominoManager.h"
#include "HoldManager.h"
#include "GameStats.h"
#include "GameStage.h"

class GamePlayStage : public GameStage
{
public:
    GamePlayStage();
    virtual ~GamePlayStage();

    GameResult GetGameResult() const;
    eStageType Update(double deltaTime) override;
    void Render(HDC memoryDeviceContext) override;

private:
    void drawTetrominoBlocks(HDC memoryDeviceContext, Vector2 drawLeftTopVector, const std::vector<Position>& tetrominoBlocks);

private:
    static const float BLOCK_LENGTH;
    static const double SOFT_DROP_SPEED_DIVISOR;

    MainBoard* mMainBoard;
    TetrominoManager* mTetrominoManager;
    GameStats* mGameStats;
    TickTimer* mFallTickTimer;
    TickTimer* mRiseTickTimer;
};

