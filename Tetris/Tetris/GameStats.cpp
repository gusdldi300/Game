#include <cassert>

#include "GameStats.h"
#include "TickTimer.h"

const unsigned int GameStats::SCORES_PER_CLEAR_LINE = 100U;
const unsigned int GameStats::CLEAR_LINES_FOR_LEVEL_UP = 3U;

// Todo: Magic number
GameStats::GameStats()
    : mTotalScore(0)
    , mTotalLinesCleared(0)
    , mStageLevel(1)
    , mFallTickRate(2.0)
    , mRiseTickRate(5.0)
{
}

unsigned int GameStats::GetTotalLinesCleared() const
{
    return mTotalLinesCleared;
}

unsigned int GameStats::GetTotalScore() const
{
    return mTotalScore;
}

unsigned int GameStats::GetStageLevel() const
{
    return mStageLevel;
}

double GameStats::GetFallTickRate() const
{
    return mFallTickRate;
}

GameResult GameStats::GetResult() const
{
    GameResult result;
    result.Score = mTotalScore;
    result.LineCleared = mTotalLinesCleared;
    result.Level = mStageLevel;

    return result;
}

bool GameStats::HasRiseTicked(TickTimer* tickTimer) const
{
    if (tickTimer->GetAccumulatedTime() >= mRiseTickRate)
    {
        return true;
    }

    return false;
}

void GameStats::UpdateInformationsFrom(unsigned int linesClearedCount)
{
    mTotalLinesCleared += linesClearedCount;
    mTotalScore += (linesClearedCount * SCORES_PER_CLEAR_LINE);

    mStageLevel = (mTotalLinesCleared / CLEAR_LINES_FOR_LEVEL_UP) + 1;
    
    mFallTickRate /= mStageLevel;
    mRiseTickRate /= mStageLevel;
}
