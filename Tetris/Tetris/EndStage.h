#pragma once

#include <string>
#include <vector>

#include "GameResult.h"
#include "MenuStage.h"

class EndStage : public MenuStage
{
public:
    EndStage();
    virtual ~EndStage() = default;

    void UpdateResult(const GameResult& result);

    eStageType Update(double deltaTime) override;
    void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) override;

private:
    static const std::wstring TITLE_STRING;

    static const std::vector<std::wstring> MENU_STRINGS;
    static const std::vector<std::wstring> SELECTED_MENU_STRINGS;

    GameResult mResult;
    unsigned int mSelectedMenu;
};

