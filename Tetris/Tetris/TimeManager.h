#pragma once

#include <wtypes.h>

// Todo: 싱글턴으로 만들지 않아도 괜찮을 듯
class TimeManager
{
public:
    static void CreateInstance();
    static void DeleteInstance();
    static TimeManager* GetInstance();

    double GetFrameDeltaTime() const;
    bool HasRiseTicked() const;
    void ResetTick();
    
    void Update();

private:
    TimeManager();
    ~TimeManager() = default;

private:
    static TimeManager* mTimeManager;
    
    LARGE_INTEGER mCount;
    LARGE_INTEGER mPrevCount;
    LARGE_INTEGER mFrequency;

    double mFrameDeltaTime;
    double mAccumulatedFrameDeltaTime;
    unsigned int mFrameCount;

    double mFallTickRate;
    bool mbTicked;
};

