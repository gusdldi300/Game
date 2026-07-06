#pragma once

#include <vector>
#include <string>

#include "MenuStage.h"

class StartStage final : public MenuStage
{
public:
    StartStage();
    virtual ~StartStage() = default;

    eStageType Update(double deltaTime) override;
    //void Render(HDC memoryDeviceContext) override;

private:
    static const Vector2 LEFT_TOP_DRAW_VECTOR;

    static const std::wstring TITLE_STRING;
    static const std::vector<std::wstring> MENU_STRINGS;
    static const std::vector<std::wstring> SELECTED_MENU_STRINGS;
};

