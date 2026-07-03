#pragma once

#include <wtypes.h>
    
#include "Vector2.h"
#include "eStatgeType.h"

class GameStage
{
public:
    GameStage(Vector2 drawStartVector);
    virtual ~GameStage() = default;

    virtual eStageType Update(double deltaTime) = 0;
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) = 0;

protected:
    Vector2 mDrawStartVector;
};

