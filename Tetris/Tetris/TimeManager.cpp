
#include <cassert>
#include <tchar.h>

#include "Engine.h"
#include "TimeManager.h"
#include <iostream>

TimeManager* TimeManager::mTimeManager = nullptr;

void TimeManager::CreateInstance()
{
    assert(mTimeManager == nullptr);

    mTimeManager = new TimeManager();

    assert(mTimeManager != nullptr);
}

void TimeManager::DeleteInstance()
{
    assert(mTimeManager != nullptr);

    delete mTimeManager;
    mTimeManager = nullptr;
}

TimeManager* TimeManager::GetInstance()
{
    assert(mTimeManager != nullptr);

    return mTimeManager;
}

TimeManager::TimeManager()
    : mFrameDeltaTime(0.0)
    , mAccumulatedFrameDeltaTime(0.0)
    , mFrameCount(0)
    , mFallTickRate(0.5f)
    , mbTicked(false)
{
    mPrevCount = { 0, };

    QueryPerformanceCounter(&mCount);
    QueryPerformanceFrequency(&mFrequency);
}

double TimeManager::GetFrameDeltaTime() const
{
    return mFrameDeltaTime;
}

bool TimeManager::HasRiseTicked() const
{
    return mbTicked;
}

void TimeManager::ResetTick()
{
    mbTicked = false;
}

void TimeManager::Update()
{
    // Todo: 필요 없는 멤버 변수가 있음
    // Todo: Update() 함수가 Progress()가 아닌, 다른 곳에서 호출될 경우, 
    // 저장되는 값들이 모두 이상하게 변할듯
    // 수정 필요함
    QueryPerformanceCounter(&mCount);
    
    mFrameDeltaTime = (static_cast<double>(mCount.QuadPart - mPrevCount.QuadPart) / static_cast<double>(mFrequency.QuadPart));
    mAccumulatedFrameDeltaTime += mFrameDeltaTime;

    ++mFrameCount;
    mPrevCount = mCount;

    if (mAccumulatedFrameDeltaTime >= mFallTickRate)
    {
        unsigned int framesPerSec = mFrameCount;
        mAccumulatedFrameDeltaTime = 0;
        mFrameCount = 0;

        wchar_t printBuffer[255];
        swprintf_s(printBuffer, L"FPS: %d, Frame Delta Time: %f", framesPerSec, mFrameDeltaTime);
    
        SetWindowText(Engine::GetInstance()->GetWindowHandle(), printBuffer);
        //std::cout << "FPS: " << framesPerSec << ", Frame Delta Time: " << mFrameDeltaTime << std::endl;

        mbTicked = true;
    }
}

