
#include <cassert>
#include <string>

#include "StartStage.h"
#include "KeyManager.h"

const Vector2 StartStage::LEFT_TOP_DRAW_VECTOR = { 550.f, 250.f };

const std::wstring StartStage::TITLE_STRING = L"----- TETRIS -----";

// Todo: Add LOAD
const std::vector<std::wstring> StartStage::MENU_STRINGS =
{
    L"  GAME START",
    //L"  SETTINGS",
    L"  EXIT"
};

const std::vector<std::wstring> StartStage::SELECTED_MENU_STRINGS =
{
    L"¢º GAME START",
    //L"¢º SETTINGS",
    L"¢º EXIT"
};

StartStage::StartStage()
    : MenuStage(LEFT_TOP_DRAW_VECTOR, TITLE_STRING, MENU_STRINGS, SELECTED_MENU_STRINGS)
{
}

eStageType StartStage::Update(double deltaTime)
{
    selectMenu();

    if (KeyManager::GetInstance()->GetKeyState(eKey::Return) == eKeyState::Press)
    {
        // Todo: Magic number
        switch (mSelectedMenu)
        {
        case 0: 
            return eStageType::Play;
        case 1: 
            //return eStageType::Setting;
        case 2:
            PostQuitMessage(0); // Todo: Check

            break;
        default:
            assert(false);
        }
    }

    return eStageType::Start;
}

