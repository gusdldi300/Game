#pragma once

#include <wtypes.h>

#include "GameStage.h"
#include "GamePlayStage.h"

class Engine
{
public:
    static void CreateInstance(HWND hWindow, POINT resolution);
    static void DeleteInstance();
    static Engine* GetInstance();

    HWND GetWindowHandle() const;
    POINT GetWindowResolution() const;

    void Run();

private:
    Engine(HWND hWindow, POINT resolution);
    ~Engine();

    void update();
    void render();

private:
    static Engine* mEngine;

    HWND mhWindow;
    POINT mWindowResolution;
    HDC mhWindowDeviceContext;

    HBITMAP mhBitMap;
    HDC mhMemoryDeviceContext;

    std::vector<GameStage*> mGameStages;
    GameStage* mCurrentStage;

    bool mbGameOn;
    LARGE_INTEGER mTimerFrequency;
    LARGE_INTEGER mPrevTime;
};

