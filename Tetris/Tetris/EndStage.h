#pragma once

#include <string>
#include <vector>

#include "GameResult.h"
#include "MenuStage.h"

class EndStage final : public MenuStage
{
public:
    EndStage();
    virtual ~EndStage() = default;

    void UpdateResult(const GameResult& result);

    eStageType Update(double deltaTime) override;
    void Render(HDC memoryDeviceContext) override;

private:
    static const Vector2 LEFT_TOP_DRAW_VECTOR;

    static const std::wstring TITLE_STRING;
    static const std::vector<std::wstring> MENU_STRINGS;
    static const std::vector<std::wstring> SELECTED_MENU_STRINGS;

    GameResult mResult;
};
