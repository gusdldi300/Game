
#include <cassert>

#include "Engine.h"

#include "KeyManager.h"
#include "GameStage.h"
#include "StartStage.h"
#include "EndStage.h"

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

// Calls every frame
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

    // Initialize stages
    {
        mGameStages.push_back(new StartStage());
        mGameStages.push_back(new GamePlayStage());
        mGameStages.push_back(new EndStage());

        mCurrentStage = mGameStages[static_cast<unsigned int>(eStageType::Start)];
    }
}

Engine::~Engine()
{
    ReleaseDC(mhWindow, mhWindowDeviceContext);

    for (unsigned int i = 0; i < mGameStages.size(); ++i)
    {
        delete mGameStages[i];
    }
    
    mCurrentStage = nullptr;
}

void Engine::update()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double deltaTime = static_cast<double>(currentTime.QuadPart - mPrevTime.QuadPart) / mTimerFrequency.QuadPart;
    mPrevTime = currentTime;

    assert(mCurrentStage != nullptr);
    eStageType stageType = mCurrentStage->Update(deltaTime);
    unsigned int currentStageIndex = static_cast<unsigned int>(stageType);

    if (stageType == eStageType::End)
    {
        unsigned int playStageIndex = static_cast<unsigned int>(eStageType::Play);
        GamePlayStage& gamePlayStage = *static_cast<GamePlayStage*>(mGameStages[playStageIndex]);

        EndStage* endStage = static_cast<EndStage*>(mGameStages[currentStageIndex]);
        endStage->UpdateResult(gamePlayStage.GetGameResult());

        gamePlayStage.ResetGame();
    }

    mCurrentStage = mGameStages[currentStageIndex];
}

void Engine::render()
{
    int windowResolutionX = static_cast<int>(mWindowResolution.x);
    int windowResolutionY = static_cast<int>(mWindowResolution.y);


    Rectangle(mhMemoryDeviceContext, -1, -1, windowResolutionX + 1, windowResolutionY + 1);

    mCurrentStage->Render(mhMemoryDeviceContext);

    BitBlt(mhWindowDeviceContext, 0, 0, windowResolutionX, windowResolutionY,
        mhMemoryDeviceContext, 0, 0, SRCCOPY);
}
