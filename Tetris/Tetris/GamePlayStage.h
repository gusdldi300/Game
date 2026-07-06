#pragma once

#include "MainBoard.h"
#include "TetrominoManager.h"
#include "GameStat.h"
#include "GameStage.h"

class GamePlayStage final : public GameStage
{
public:
    GamePlayStage();
    virtual ~GamePlayStage();

    GameResult GetGameResult() const;
    eStageType Update(double deltaTime) override;
    void Render(HDC memoryDeviceContext) override;

    void ResetGame();

public:
    

private:
    enum
    {
        DRAW_NEXT_TETROMINOS_COUNT = 5U,

        WALLS_COUNT = 2U,
        DRAW_BOARD_ROW_SIZE = MainBoard::BOARD_ROW_SIZE + WALLS_COUNT,
        DRAW_BOARD_COL_SIZE = MainBoard::BOARD_COL_SIZE + WALLS_COUNT,
    };

    static const float DRAW_BOARD_HEIGHT;
    static const float DRAW_BOARD_WIDTH;
    static const float BOX_HALF_LENGTH;

    static const double SOFT_DROP_SPEED_DIVISOR;

    MainBoard* mMainBoard;
    TetrominoManager* mTetrominoManager;
    GameStat* mGameStats;
    TickTimer* mFallTickTimer;
    TickTimer* mRiseTickTimer;
};

