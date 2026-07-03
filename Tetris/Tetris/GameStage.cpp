

#include <cassert>

#include "EndStage.h"
#include "KeyManager.h"

const float MenuStage::STRING_DRAW_OFFSET = 50.f;
const float MenuStage::BOX_LENGTH = 250.f;

GameStage::GameStage(Vector2 drawStartVector)
    : mDrawStartVector(drawStartVector)
{
}

void GameStage::drawScores(HDC memoryDeviceContext, Vector2 leftTopVector, const std::vector<std::wstring>& statStrings) const
{
    const unsigned int STRING_BOX_HEIGHT = STRING_DRAW_OFFSET * (statStrings.size() + 1);

    Rectangle(memoryDeviceContext,
        leftTopVector.X,
        leftTopVector.Y,
        leftTopVector.X + BOX_LENGTH,
        leftTopVector.Y + STRING_BOX_HEIGHT);

    Vector2 drawStringVector = leftTopVector;
    drawStringVector.X += STRING_DRAW_OFFSET;

    for (const std::wstring& statString : statStrings)
    {
        drawStringVector.Y += STRING_DRAW_OFFSET;

        TextOut(memoryDeviceContext,
            drawStringVector.X,
            drawStringVector.Y,
            statString.c_str(), statString.length());
    }
}


