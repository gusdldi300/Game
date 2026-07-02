#pragma once

class GameStats
{
public:
    GameStats();
    virtual ~GameStats() = default;

    unsigned int GetTotalScore() const;
    unsigned int GetStageLevel() const;

    bool HasStageLevelUp() const;
    void ProcessLineClear(unsigned int lineClearCount);

private:
    static const unsigned int STAGES_COUNT;
    static const unsigned int STAGE_LEVEL_REACH_SCORES[];
    static const unsigned int MAX_STAGE_LEVEL;

    unsigned int mTotalScore;
    unsigned int mStageLevel;
    
    // Todo: 변수 추가하는게 맞는지 확인
    bool mbLevelUp;
};

