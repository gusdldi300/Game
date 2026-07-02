
#include <string>

#include "framework.h"
#include "GameStage.h"

const float GameStage::BLOCK_LENGTH = 30.f;

GameStage::GameStage(Vector2 leftTopPosition)
    : mLeftTopPosition(leftTopPosition)
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

