#include <cassert>

#include "GameStats.h"

// Todo: duplicated
const unsigned int GameStats::MAX_STAGE_LEVEL = 5U;
const unsigned int GameStats::STAGES_COUNT = 5U;
const unsigned int GameStats::STAGE_LEVEL_REACH_SCORES[] =
{
    3, 6, 10, 15
};

GameStats::GameStats()
    : mTotalScore(0)
    , mStageLevel(1)
    , mbLevelUp(false)
{
}

bool GameStats::HasStageLevelUp() const
{
    return mbLevelUp;
}

void GameStats::ProcessLineClear(unsigned int lineClearCount)
{
    mbLevelUp = false;

    unsigned int addScore = 0;
    switch (lineClearCount)
    {
    case 0:
        // Do nothing
        return; // Todo: Check later
    case 1:
        addScore = 1;
        break;
    case 2:
        addScore = 4;
        break;
    case 3:
        addScore = 6;
        break;
    case 4:
        addScore = 12;
        break;
    default:
        assert(false);
    }

    mTotalScore += addScore;

    unsigned int level = mStageLevel;
    for (unsigned int levelIndex = 0; levelIndex < MAX_STAGE_LEVEL - 1; ++levelIndex)
    {
        if (mTotalScore <= STAGE_LEVEL_REACH_SCORES[levelIndex])
        {
            level = levelIndex + 1;

            break;
        }
    }

    if (level > mStageLevel)
    {
        mbLevelUp = true;

        mStageLevel = level;
    }
}
