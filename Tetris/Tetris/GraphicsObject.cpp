
#include "framework.h"
#include "GraphicsObject.h"
#include "KeyManager.h"
#include "TimeManager.h"

GraphicsObject::GraphicsObject(Vector2 position, float width, float height)
    : mPosition(position)
    , mWidth(width)
    , mHeight(height)
{
}

//void GraphicsObject::Update()
//{
    //float frameDeltaTime = static_cast<float>(TimeManager::GetInstance()->GetFrameDeltaTime());
    //float frameDeltaTime = 10.f;
    //const float MOVE_DISTANCE_PER_SECOND = 10.f;

    //KeyManager* keyManager = KeyManager::GetInstance();

    // Todo: Move to GraphicsObject
    //if (keyManager->GetKeyState(eKey::Left) == eKeyState::Press)
    //{
    //    mPosition.X -= (MOVE_DISTANCE_PER_SECOND * frameDeltaTime);
    //}

    //if (keyManager->GetKeyState(eKey::Right) == eKeyState::Press)
    //{
    //    mPosition.X += (MOVE_DISTANCE_PER_SECOND * frameDeltaTime);
    //}
//}

// Todo: HDC 인자 중 하나는 전달할 필요 없음
//void GraphicsObject::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
//{
    // Todo: 사각형 크기 수정 필요함
    //Rectangle(memoryDeviceContext, -1, -1, windowResolution.x + 1, windowResolution.y + 1);

    // Todo: 코드 중복
    //Rectangle(memoryDeviceContext, mPosition.X, mPosition.Y, mPosition.X + mWidth, mPosition.Y + mHeight);

    //BitBlt(windowDeviceContext, 0, 0, windowResolution.x, windowResolution.y, memoryDeviceContext, 0, 0, SRCCOPY);
//}