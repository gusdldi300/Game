#pragma once

#include "GameResult.h"

class TickTimer;

class GameStats
{
public:
    GameStats();
    virtual ~GameStats() = default;

    unsigned int GetTotalLinesCleared() const;
    unsigned int GetTotalScore() const;
    unsigned int GetStageLevel() const;
    double GetFallTickRate() const;

    GameResult GetResult() const;
    bool HasRiseTicked(TickTimer* tickTimer) const;
    void UpdateInformationsFrom(unsigned int linesClearedCount);
    void Reset();

private:
    static const unsigned int SCORES_PER_CLEAR_LINE;
    static const unsigned int CLEAR_LINES_FOR_LEVEL_UP;
    static const unsigned int STAGE_START_LEVEL;
    static const double FALL_TICK_START_RATE;
    static const double RISE_TICK_START_RATE;

    unsigned int mTotalScore;
    unsigned int mTotalLinesCleared;
    unsigned int mStageLevel;
    double mFallTickRate;
    double mRiseTickRate;
};

