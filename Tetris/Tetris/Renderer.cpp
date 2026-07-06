
#include <cassert>
#include <windows.h>

#include "Renderer.h"

#include "GamePlayStage.h"

const float Renderer::DRAW_STRING_OFFSET = 50.f;

const float Renderer::DRAW_OFFSET = 30.f;
const float Renderer::BOX_LENGTH = 250.f;
const float Renderer::BLOCK_LENGTH = 30.f;

Renderer* Renderer::mRenderer = nullptr;

void Renderer::CreateInstance()
{
    assert(mRenderer == nullptr);

    mRenderer = new Renderer();

    assert(mRenderer != nullptr);
}

void Renderer::DeleteInstance()
{
    assert(mRenderer != nullptr);

    delete mRenderer;
    mRenderer = nullptr;
}

Renderer* Renderer::GetInstance()
{
    assert(mRenderer != nullptr);

    return mRenderer;
}

Renderer::Renderer()
{
    mhBrushes.reserve(static_cast<size_t>(eColor::None));

    mhBrushes.push_back(CreateSolidBrush(RGB(0, 162, 232)));   // Blue (I)
    mhBrushes.push_back(CreateSolidBrush(RGB(255, 242, 0)));   // Yellow (O)
    mhBrushes.push_back(CreateSolidBrush(RGB(163, 73, 164)));  // Purple (T)
    mhBrushes.push_back(CreateSolidBrush(RGB(255, 127, 39)));  // Orange (L)
    mhBrushes.push_back(CreateSolidBrush(RGB(63, 72, 204)));   // DarkBlue (J)
    mhBrushes.push_back(CreateSolidBrush(RGB(34, 177, 76)));   // Green (S)
    mhBrushes.push_back(CreateSolidBrush(RGB(237, 28, 36)));   // Red (Z)
    mhBrushes.push_back(CreateSolidBrush(RGB(195, 195, 195))); // Gray (Ghost)
    mhBrushes.push_back(CreateSolidBrush(RGB(80, 80, 80)));    // DarkGray (Dead Block)
    mhBrushes.push_back(CreateSolidBrush(RGB(40, 40, 40)));    // Light Black (Wall)
    mhBrushes.push_back(CreateSolidBrush(RGB(255, 255, 255)));    // None
}

Renderer::~Renderer()
{
    for (HBRUSH hBrush : mhBrushes)
    {
        DeleteObject(hBrush);
    }

    mhBrushes.clear();
}

void Renderer::DrawStatsBoard(HDC deviceContext, const Vector2& leftTopVector, const std::vector<std::wstring>& statStrings) const
{
    // Todo: Strings size maybe const
    const float STRING_BOX_HEIGHT = DRAW_STRING_OFFSET * (statStrings.size() + 1);

    DrawRectangle(deviceContext,
        leftTopVector.X,
        leftTopVector.Y,
        leftTopVector.X + BOX_LENGTH,
        leftTopVector.Y + STRING_BOX_HEIGHT + DRAW_OFFSET);

    Vector2 drawStringVector = leftTopVector;
    drawStringVector.X += DRAW_STRING_OFFSET;

    for (const std::wstring& statString : statStrings)
    {
        drawStringVector.Y += DRAW_STRING_OFFSET;
        DrawString(deviceContext, drawStringVector, statString);
    }
}

void Renderer::DrawBlocks(HDC memoryDeviceContext, const Vector2& leftTopVector, eColor color, const std::vector<Position>& blocks) const
{
    for (const Position& blockPosition : blocks)
    {
        DrawBlock(memoryDeviceContext, leftTopVector, color, blockPosition);
    }
}

