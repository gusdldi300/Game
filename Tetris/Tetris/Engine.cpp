
#include <cassert>

#include "Engine.h"
#include "TimeManager.h"
#include "KeyManager.h"

#include "Block.h"

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
void Engine::Progress(std::vector<GraphicsGrid*> objects)
{
    /*
    TimeManager::GetInstance()->Update();
    KeyManager::GetInstance()->Update();
    
    for (GraphicsObject* object : objects)
    {
        object->Update();
    }
    */

    for (GraphicsGrid* object : objects)
    {
        object->Render(mhWindowDeviceContext, mhMemoryDeviceContext, mWindowResolution);
    }
}

Engine::Engine(HWND hWindow, POINT resolution)
    : mhWindow(hWindow)
    , mWindowResolution(resolution)
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

    // Todo: Initialize()로 옮기는게 좋을지도
    {
        TimeManager::CreateInstance();
        KeyManager::CreateInstance();

        //GraphicsObject* object = new Block();
        //mGraphicsObjects.push_back(object);
    }
}

Engine::~Engine()
{
    ReleaseDC(mhWindow, mhWindowDeviceContext);

    /*
    unsigned int deleteObjectsSize = mGraphicsObjects.size();
    for (unsigned int i = 0; i < deleteObjectsSize; ++i)
    {
        delete mGraphicsObjects[i];
    }
    */
}

void Engine::update()
{
    /*
    for (GraphicsObject* object : mGraphicsObjects)
    {
        object->Update();
    }
    */
}

void Engine::render()
{
    /*
    for (GraphicsObject* object : mGraphicsObjects)
    {
        object->Render(mhWindowDeviceContext, mhMemoryDeviceContext, mWindowResolution);
    }
    */
}
