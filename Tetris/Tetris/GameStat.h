#pragma once

#include "GameResult.h"

class TickTimer;

class GameStat
{
public:
    GameStat();
    virtual ~GameStat() = default;

    unsigned int GetTotalLinesCleared() const;
    unsigned int GetTotalScore() const;
    unsigned int GetStageLevel() const;
    double GetFallTickRate() const;

    GameResult GetResult() const;
    bool HasRiseTicked(TickTimer* tickTimer) const;
    void UpdateInformationsFrom(unsigned int linesClearedCount);
    void Reset();

private:
    enum
    {
        SCORES_PER_CLEAR_LINE = 100U,
        CLEAR_LINES_FOR_LEVEL_UP = 3U,
        STAGE_START_LEVEL = 1U
    };
    
    static const double FALL_TICK_START_RATE;
    static const double RISE_TICK_START_RATE;

    unsigned int mTotalScore;
    unsigned int mTotalLinesCleared;
    unsigned int mStageLevel;
    double mFallTickRate;
    double mRiseTickRate;
};

