#pragma once

#include <vector>
#include <string>

#include "GameStage.h"

// Todo: 외부에서 생성 못하게 하기
class MenuStage : public GameStage
{
public:
    MenuStage(Vector2 drawStartVector, std::wstring titleString, std::vector<std::wstring> menuStrings, std::vector<std::wstring> selectedMenuStrings);
    virtual ~MenuStage() = default;

    virtual eStageType Update(double deltaTime) = 0;
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) override;
    
protected:
    void selectMenu();

private:
    static const float STRING_DRAW_OFFSET;

    std::wstring mTitleString;
    std::vector<std::wstring> mMenuStrings;
    std::vector<std::wstring> mSelectedMenuStrings;

    unsigned int mSelectedMenu;
};


