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
    static const float DRAW_STRING_OFFSET;

    static const float DRAW_OFFSET;
    static const float BOX_LENGTH;

    inline void drawString(HDC deviceContext, const Vector2& leftTopVector, const std::wstring& wstring) const;
    inline void drawRectangle(HDC deviceContext, float leftTopX, float leftTopY, float rightBottomX, float rightBottomY) const;
    void drawStatsBoard(HDC deviceContext, const Vector2& leftTopVector, const std::vector<std::wstring>& statStrings) const;

protected:
    Vector2 mDrawStartVector;
};

inline void GameStage::drawString(HDC deviceContext, const Vector2& leftTopVector, const std::wstring& wstring) const
{
    TextOut(deviceContext,
        static_cast<int>(leftTopVector.X),
        static_cast<int>(leftTopVector.Y),
        wstring.c_str(), static_cast<int>(wstring.length()));
}

inline void GameStage::drawRectangle(HDC deviceContext, float leftTopX, float leftTopY, float rightBottomX, float rightBottomY) const
{
    Rectangle(deviceContext,
        static_cast<int>(leftTopX),
        static_cast<int>(leftTopY),
        static_cast<int>(rightBottomX),
        static_cast<int>(rightBottomY));
}

