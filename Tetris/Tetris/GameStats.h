#pragma once

class TickTimer;

class GameStats
{
public:
    GameStats();
    virtual ~GameStats() = default;

    unsigned int GetTotalLinesCleared() const;
    unsigned int GetTotalScore() const;
    unsigned int GetStageLevel() const;
    bool Ticked(TickTimer* tickTimer) const;

    void UpdateInformationsFrom(unsigned int linesClearedCount);

private:
    static const unsigned int SCORES_PER_CLEAR_LINE;
    static const unsigned int CLEAR_LINES_FOR_LEVEL_UP;

    unsigned int mTotalScore;
    unsigned int mTotalLinesCleared;
    unsigned int mStageLevel;
    double mTickRate;
};

