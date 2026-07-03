#pragma once

#include <wtypes.h>
#include <vector>
#include <string>
    
#include "Vector2.h"
#include "eStatgeType.h"

class GameStage
{
public:
    GameStage(Vector2 drawStartVector);
    virtual ~GameStage() = default;

    virtual eStageType Update(double deltaTime) = 0;
    virtual void Render(HDC memoryDeviceContext) = 0;

protected:
    static const float STRING_DRAW_OFFSET;
    static const float BOX_LENGTH;

    void drawScores(HDC memoryDeviceContext, Vector2 leftTopVector, const std::vector<std::wstring>& statStrings) const;

protected:
    Vector2 mDrawStartVector;
};

