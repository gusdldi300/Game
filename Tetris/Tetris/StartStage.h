#pragma once

#include <vector>
#include <string>

#include "GameStage.h"

class StartStage : public GameStage
{
public:
    StartStage();
    virtual ~StartStage() = default;

    eStageType Update(double deltaTime) override;
    void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) override;

private:
    static const Vector2 TITLE_START_VECTOR;
    // Todo: code duplicate
    static const float STRING_DRAW_OFFSET;

    static const std::wstring TITLE_STRING;
    
    static const std::vector<std::wstring> MENU_STRINGS;
    static const std::vector<std::wstring> SELECTED_MENU_STRINGS;
 
    unsigned int mSelectedMenu;
};

