#pragma once

#include <wtypes.h>
    
#include "Vector2.h"

class GameStage
{
public:
    GameStage(Vector2 leftTopPosition);
    virtual ~GameStage() = default;

    virtual void Update(double deltaTime) = 0;
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) = 0;

protected:
    static const float BLOCK_LENGTH;

    Vector2 mLeftTopPosition;
};

