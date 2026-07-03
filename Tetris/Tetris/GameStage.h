#pragma once

#include <wtypes.h>
    
#include "Vector2.h"
#include "eStatgeType.h"

// Tod: Make as Interface
class GameStage
{
public:
    GameStage() = default;
    virtual ~GameStage() = default;

    virtual eStageType Update(double deltaTime) = 0;
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) = 0;

protected:
    static const float BLOCK_LENGTH;
};

