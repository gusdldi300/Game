
#include "MenuStage.h"
#include "KeyManager.h"
#include <cassert>

// Todo: Use constexpr

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
    TextOut(memoryDeviceContext, mDrawStartVector.X, mDrawStartVector.Y, mTitleString.c_str(), mTitleString.length());

    Vector2 selectMenuStartVector = mDrawStartVector;
    for (unsigned int menuIndex = 0; menuIndex < mMenuStrings.size(); ++menuIndex)
    {
        selectMenuStartVector.Y += STRING_DRAW_OFFSET;

        TextOut(memoryDeviceContext, selectMenuStartVector.X, selectMenuStartVector.Y,
            (mSelectedMenu == menuIndex) ? mSelectedMenuStrings[menuIndex].c_str() : mMenuStrings[menuIndex].c_str(),
            mMenuStrings[menuIndex].length());
    }
}

void MenuStage::selectMenu()
{
    KeyManager* keyManager = KeyManager::GetInstance();

    if (keyManager->GetKeyState(eKey::Up) == eKeyState::Press)
    {
        mSelectedMenu = (mSelectedMenu + mMenuStrings.size() - 1) % mMenuStrings.size();
    }
    else if (keyManager->GetKeyState(eKey::Down) == eKeyState::Press)
    {
        mSelectedMenu = (mSelectedMenu + 1) % mMenuStrings.size();
    }
}

