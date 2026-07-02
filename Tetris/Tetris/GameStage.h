#pragma once

#include <wtypes.h>
    
#include "Vector2.h"

class GameStage
{
public:
    GameStage(Vector2 leftTopPosition);
    virtual ~GameStage() = default;

    //virtual void Update() = 0;
    // Todo: HDC 인자 중 하나는 전달할 필요 없음
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) = 0;

protected:
    static const float BLOCK_LENGTH;

    Vector2 mLeftTopPosition;
};

