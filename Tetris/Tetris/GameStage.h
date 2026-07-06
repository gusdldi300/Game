#pragma once

#include <wtypes.h>
#include <vector>
#include <string>
    
#include "Vector2.h"
#include "eStatgeType.h"
#include "Renderer.h"

class GameStage
{
public:
    GameStage(Vector2 drawStartVector);
    virtual ~GameStage() = default;

    virtual eStageType Update(double deltaTime) = 0;
    virtual void Render(HDC memoryDeviceContext) = 0;

protected:
    Vector2 mDrawStartVector;
};


