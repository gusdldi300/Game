
#include <cassert>

#include "Block.h"

#include "Engine.h"
#include "GameStage.h"
#include "TimeManager.h"
#include "KeyManager.h"


// Todo: temp object

Engine* Engine::mEngine = nullptr;

Engine* Engine::GetInstance()
{
    assert(mEngine != nullptr);
    
    return mEngine;
}

void Engine::CreateInstance(HWND hWindow, POINT resolution)
{
    assert(mEngine == nullptr);
    
    mEngine = new Engine(hWindow, resolution);
    
    assert(mEngine != nullptr);
}

void Engine::DeleteInstance()
{
    assert(mEngine != nullptr);

    delete mEngine;
    mEngine = nullptr;
}

HWND Engine::GetWindowHandle() const
{
    return mhWindow;
}

POINT Engine::GetWindowResolution() const
{
    return mWindowResolution;
}

// 매 프레임마다 호출됨
void Engine::Run()
{
    MSG msg;

    while (mbGameOn)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);

            continue;
        }

        KeyManager::GetInstance()->Update();
        update();

        render();
    }
}

Engine::Engine(HWND hWindow, POINT resolution)
    : mhWindow(hWindow)
    , mWindowResolution(resolution)
    , mbGameOn(true)
    , mGameplayStage(new GamePlayStage())
    , mCurrentStage(mGameplayStage)
    //mStartStage = new StartStage();
{
    // Create window
    {
        mhWindowDeviceContext = GetDC(mhWindow);

        RECT windowSizeRect = { 0, 0, mWindowResolution.x, mWindowResolution.y };
        AdjustWindowRect(&windowSizeRect, WS_OVERLAPPEDWINDOW, true);

        const int WINDOW_START_POSITION = 100;
        SetWindowPos(mhWindow, nullptr, WINDOW_START_POSITION, WINDOW_START_POSITION,
            windowSizeRect.right - windowSizeRect.left,
            windowSizeRect.bottom - windowSizeRect.top, 0);
    }

    // Create bit map
    {
        mhBitMap = CreateCompatibleBitmap(mhWindowDeviceContext, mWindowResolution.x, mWindowResolution.y);
        mhMemoryDeviceContext = CreateCompatibleDC(mhWindowDeviceContext);

        HBITMAP hOldBitMap = static_cast<HBITMAP>(SelectObject(mhMemoryDeviceContext, mhBitMap));
        DeleteObject(hOldBitMap);
    }

    // Initialize timer
    {
        QueryPerformanceFrequency(&mTimerFrequency);
        QueryPerformanceCounter(&mPrevTime);
    }

}

Engine::~Engine()
{
    ReleaseDC(mhWindow, mhWindowDeviceContext);

    delete mGameplayStage;
    // delete mStatStage;
    
    mCurrentStage = nullptr;
}

void Engine::update()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double deltaTime = static_cast<double>(currentTime.QuadPart - mPrevTime.QuadPart) / mTimerFrequency.QuadPart;
    mPrevTime = currentTime;

    // Todo: 스테이지 전환 키 입력 검사 
    // HandleGlobalInput(); 

    assert(mCurrentStage != nullptr);
    mCurrentStage->Update(deltaTime);
}

void Engine::render()
{
    Rectangle(mhMemoryDeviceContext, -1, -1, mWindowResolution.x + 1, mWindowResolution.y + 1);

    mCurrentStage->Render(mhWindowDeviceContext, mhMemoryDeviceContext, mWindowResolution);

    BitBlt(mhWindowDeviceContext, 0, 0, mWindowResolution.x, mWindowResolution.y,
        mhMemoryDeviceContext, 0, 0, SRCCOPY);
}
