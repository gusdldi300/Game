#pragma once

#include <vector>
#include <string>

#include "GameStage.h"

class MenuStage : public GameStage
{
public:
    MenuStage(Vector2 drawleftTopVector, std::wstring titleString, std::vector<std::wstring> menuStrings, std::vector<std::wstring> selectedMenuStrings);
    virtual ~MenuStage() = default;

    virtual eStageType Update(double deltaTime) = 0;
    virtual void Render(HDC memoryDeviceContext) override;
    
protected:
    void selectMenu();
    
protected:
    unsigned int mSelectedMenu;

private:
    std::wstring mTitleString;
    std::vector<std::wstring> mMenuStrings;
    std::vector<std::wstring> mSelectedMenuStrings;
};


