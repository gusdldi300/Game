
#include <cassert>

#include "EndStage.h"

const float GameStage::DRAW_STRING_OFFSET = 50.f;

const float GameStage::DRAW_OFFSET = 30.f;
const float GameStage::BOX_LENGTH = 250.f;

GameStage::GameStage(Vector2 drawStartVector)
    : mDrawStartVector(drawStartVector)
{
}

void GameStage::drawStatsBoard(HDC deviceContext, const Vector2& leftTopVector, const std::vector<std::wstring>& statStrings) const
{
    // Todo: Strings size maybe const
    const float STRING_BOX_HEIGHT = GameStage::DRAW_STRING_OFFSET * (statStrings.size() + 1);

    drawRectangle(deviceContext,
        leftTopVector.X,
        leftTopVector.Y,
        leftTopVector.X + BOX_LENGTH,
        leftTopVector.Y + STRING_BOX_HEIGHT + DRAW_OFFSET);

    Vector2 drawStringVector = leftTopVector;
    drawStringVector.X += DRAW_STRING_OFFSET;

    for (const std::wstring& statString : statStrings)
    {
        drawStringVector.Y += DRAW_STRING_OFFSET;
        drawString(deviceContext, drawStringVector, statString);
    }
}


