#include "TickTimer.h"

TickTimer::TickTimer()
    : mAccumulatedTime(0.0)
{
}

double TickTimer::GetAccumulatedTime() const
{
    return mAccumulatedTime;
}

void TickTimer::AccumulateTime(double time)
{
    mAccumulatedTime += time;
}

void TickTimer::ResetTimer()
{
    mAccumulatedTime = 0.0;
}
