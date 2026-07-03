
#include <cassert>
#include <string>

#include "StartStage.h"
#include "KeyManager.h"

const Vector2 StartStage::TITLE_START_VECTOR = { 550.f, 250.f };
const float StartStage::STRING_DRAW_OFFSET = 50.f;

const std::wstring StartStage::TITLE_STRING = L"----- TETRIS -----";

// Todo: Add LOAD
const std::vector<std::wstring> StartStage::MENU_STRINGS =
{
    L"  GAME START",
    L"  SETTINGS",
    L"  EXIT"
};

const std::vector<std::wstring> StartStage::SELECTED_MENU_STRINGS =
{
    L"▶ GAME START",
    L"▶ SETTINGS",
    L"▶ EXIT"
};

StartStage::StartStage()
    : GameStage()
    , mSelectedMenu(0)
{
}

eStageType StartStage::Update(double deltaTime)
{
    KeyManager* keyManager = KeyManager::GetInstance();

    // 메뉴 커서 조작
    if (keyManager->GetKeyState(eKey::Up) == eKeyState::Press)
    {
        mSelectedMenu = (mSelectedMenu + MENU_STRINGS.size() - 1) % MENU_STRINGS.size();
    }
    else if (keyManager->GetKeyState(eKey::Down) == eKeyState::Press)
    {
        mSelectedMenu = (mSelectedMenu + 1) % MENU_STRINGS.size();
    }

    if (keyManager->GetKeyState(eKey::Return) == eKeyState::Press)
    {
        // Todo: Magic number
        switch (mSelectedMenu)
        {
        case 0: 
            // Todo: Reset game
            return eStageType::Play;
        case 1: 
            return eStageType::Setting;
        case 2:
            PostQuitMessage(0); // Todo: Check

            break;
        default:
            assert(false);
        }
    }

    return eStageType::Start;
}

void StartStage::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    TextOut(memoryDeviceContext, TITLE_START_VECTOR.X, TITLE_START_VECTOR.Y, TITLE_STRING.c_str(), TITLE_STRING.length());

    Vector2 selectMenuStartVector = TITLE_START_VECTOR;
    for (unsigned int menuIndex = 0; menuIndex < MENU_STRINGS.size(); ++menuIndex)
    {
        selectMenuStartVector.Y += STRING_DRAW_OFFSET;

        TextOut(memoryDeviceContext, selectMenuStartVector.X, selectMenuStartVector.Y, 
                (mSelectedMenu == menuIndex) ? SELECTED_MENU_STRINGS[menuIndex].c_str() : MENU_STRINGS[menuIndex].c_str(),
                MENU_STRINGS[menuIndex].length());
    }
}
