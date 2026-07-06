
#include <cassert>

#include "MenuStage.h"
#include "KeyManager.h"

MenuStage::MenuStage(Vector2 drawStartVector, std::wstring titleString, std::vector<std::wstring> menuStrings, std::vector<std::wstring> selectedMenuStrings)
    : GameStage(drawStartVector)
    , mSelectedMenu(0)
    , mTitleString(titleString)
    , mMenuStrings(menuStrings)
    , mSelectedMenuStrings(selectedMenuStrings)
{
}

void MenuStage::Render(HDC memoryDeviceContext)
{
    // Todo: 텍스트 크기 늘리기
    drawString(memoryDeviceContext, mDrawStartVector, mTitleString);

    Vector2 selectMenuStartVector = mDrawStartVector;
    for (unsigned int menuIndex = 0; menuIndex < mMenuStrings.size(); ++menuIndex)
    {
        selectMenuStartVector.Y += DRAW_STRING_OFFSET;

        drawString(memoryDeviceContext, selectMenuStartVector,
            (mSelectedMenu == menuIndex) ? mSelectedMenuStrings[menuIndex] : mMenuStrings[menuIndex]);
    }
}

void MenuStage::selectMenu()
{
    KeyManager* keyManager = KeyManager::GetInstance();

    if (keyManager->GetKeyState(eKey::Up) == eKeyState::Press)
    {
        mSelectedMenu = (mSelectedMenu + mMenuStrings.size() - 1) % static_cast<unsigned int>(mMenuStrings.size());
    }
    else if (keyManager->GetKeyState(eKey::Down) == eKeyState::Press)
    {
        mSelectedMenu = (mSelectedMenu + 1) % static_cast<unsigned int>(mMenuStrings.size());
    }
}

