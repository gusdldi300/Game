#pragma once

class TickTimer
{
public: 
    TickTimer();
    virtual ~TickTimer() = default;

    double GetAccumulatedTime() const;
    void AccumulateTime(double time);
    void ResetTimer();

private:
    double mAccumulatedTime;
};

