#include <cassert>

#include "GameStat.h"
#include "TickTimer.h"

const unsigned int GameStat::SCORES_PER_CLEAR_LINE = 100U;
const unsigned int GameStat::CLEAR_LINES_FOR_LEVEL_UP = 3U;

const unsigned int GameStat::STAGE_START_LEVEL = 1U;
const double GameStat::FALL_TICK_START_RATE = 2.0;
const double GameStat::RISE_TICK_START_RATE = 2.0;

// Todo: Magic number
GameStat::GameStat()
    : mTotalScore(0)
    , mTotalLinesCleared(0)
    , mStageLevel(1)
    , mFallTickRate(2.0)
    , mRiseTickRate(5.0)
{
}

unsigned int GameStat::GetTotalLinesCleared() const
{
    return mTotalLinesCleared;
}

unsigned int GameStat::GetTotalScore() const
{
    return mTotalScore;
}

unsigned int GameStat::GetStageLevel() const
{
    return mStageLevel;
}

double GameStat::GetFallTickRate() const
{
    return mFallTickRate;
}

GameResult GameStat::GetResult() const
{
    GameResult result;
    result.Score = mTotalScore;
    result.LineCleared = mTotalLinesCleared;
    result.Level = mStageLevel;

    return result;
}

bool GameStat::HasRiseTicked(TickTimer* tickTimer) const
{
    if (tickTimer->GetAccumulatedTime() >= mRiseTickRate)
    {
        return true;
    }

    return false;
}

void GameStat::UpdateInformationsFrom(unsigned int linesClearedCount)
{
    mTotalLinesCleared += linesClearedCount;
    mTotalScore += (linesClearedCount * SCORES_PER_CLEAR_LINE);

    mStageLevel = (mTotalLinesCleared / CLEAR_LINES_FOR_LEVEL_UP) + 1;
    
    mFallTickRate /= mStageLevel;
    mRiseTickRate /= mStageLevel;
}

void GameStat::Reset()
{
    mTotalScore = 0;
    mTotalLinesCleared = 0;
    mStageLevel = STAGE_START_LEVEL;
    mFallTickRate = FALL_TICK_START_RATE;
    mRiseTickRate = RISE_TICK_START_RATE;
}

