#pragma once

#include <vector>
#include <string>

#include "eColor.h"
#include "Vector2.h"
#include "Position.h"

class Renderer
{
public:
    static void CreateInstance();
    static void DeleteInstance();
    static Renderer* GetInstance();

    inline void DrawString(HDC deviceContext, const Vector2& leftTopVector, const std::wstring& wstring) const;
    inline void DrawRectangle(HDC deviceContext, float leftTopX, float leftTopY, float rightBottomX, float rightBottomY) const;

    void DrawStatsBoard(HDC deviceContext, const Vector2& leftTopVector, const std::vector<std::wstring>& statStrings) const;
    void DrawBlocks(HDC memoryDeviceContext, const Vector2& leftTopVector, eColor color, const std::vector<Position>& blocks) const;

    inline void DrawBlock(HDC memoryDeviceContext, const Vector2& leftTopVector, eColor color, const Position& position) const;

private:
    Renderer();
    ~Renderer();

public:
    static const float BLOCK_LENGTH;
    static const float BOX_LENGTH;
    static const float DRAW_OFFSET;
    static const float DRAW_STRING_OFFSET;

private:
    static Renderer* mRenderer;

    std::vector<HBRUSH> mhBrushes;
};

inline void Renderer::DrawString(HDC deviceContext, const Vector2& leftTopVector, const std::wstring& wstring) const
{
    TextOut(deviceContext,
        static_cast<int>(leftTopVector.X),
        static_cast<int>(leftTopVector.Y),
        wstring.c_str(), static_cast<int>(wstring.length()));
}

inline void Renderer::DrawRectangle(HDC deviceContext, float leftTopX, float leftTopY, float rightBottomX, float rightBottomY) const
{
    Rectangle(deviceContext,
        static_cast<int>(leftTopX),
        static_cast<int>(leftTopY),
        static_cast<int>(rightBottomX),
        static_cast<int>(rightBottomY));
}

inline void Renderer::DrawBlock(HDC memoryDeviceContext, const Vector2& leftTopVector, eColor color, const Position& position) const
{
    SelectObject(memoryDeviceContext, mhBrushes[static_cast<unsigned int>(color)]);

    float drawX = leftTopVector.X + (position.GetCol() * BLOCK_LENGTH);
    float drawY = leftTopVector.Y + (position.GetRow() * BLOCK_LENGTH);

    DrawRectangle(memoryDeviceContext,
        drawX,
        drawY,
        drawX + Renderer::BLOCK_LENGTH,
        drawY + Renderer::BLOCK_LENGTH);

    SelectObject(memoryDeviceContext, mhBrushes[static_cast<unsigned int>(eColor::None)]);
}
