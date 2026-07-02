#pragma once

#include <wtypes.h>
#include <vector>

#include "GameStage.h"
#include "GamePlayStage.h"

class Engine
{
public:
    static Engine* GetInstance();
    
    static void CreateInstance(HWND hWindow, POINT resolution);
    static void DeleteInstance();

    HWND GetWindowHandle() const;
    POINT GetWindowResolution() const;

    void Run();

private:
    void update();
    void render();

private:
    Engine(HWND hWindow, POINT resolution);
    ~Engine();

private:
    static Engine* mEngine;

    HWND mhWindow;
    POINT mWindowResolution;
    HDC mhWindowDeviceContext;

    HBITMAP mhBitMap;
    HDC mhMemoryDeviceContext;

    //StartStage* mStartStage;
    GamePlayStage* mGameplayStage;

    GameStage* mCurrentStage;

    bool mbGameOn;
    LARGE_INTEGER mTimerFrequency;
    LARGE_INTEGER mPrevTime;
};

