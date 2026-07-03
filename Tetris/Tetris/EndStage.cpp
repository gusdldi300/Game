#include "EndStage.h"
#include "KeyManager.h"
#include <cassert>

// Todo: Use constexpr
// Todo: Code duplicated
const std::wstring EndStage::TITLE_STRING = L"----- GAME OVER -----";

const std::vector<std::wstring> EndStage::MENU_STRINGS =
{
    L"  RESTART GAME",
    L"  EXIT"
};

const std::vector<std::wstring> EndStage::SELECTED_MENU_STRINGS =
{
    L"¢º RESTART GAME",
    L"¢º EXIT"
};

EndStage::EndStage()
    : MenuStage({ 550.f, 250.f }, TITLE_STRING, MENU_STRINGS, SELECTED_MENU_STRINGS)
    , mResult({ 0, })
{
}

void EndStage::UpdateResult(const GameResult& result)
{
    mResult = result;
}

eStageType EndStage::Update(double deltaTime)
{
    selectMenu();

    if (KeyManager::GetInstance()->GetKeyState(eKey::Return) == eKeyState::Press)
    {
        // Todo: Magic number
        switch (mSelectedMenu)
        {
        case 0:
            // Todo: Reset game
            return eStageType::Play;
        case 1:
            PostQuitMessage(0); // Todo: Check

            break;
        default:
            assert(false);
        }
    }

    return eStageType::End;
}

void EndStage::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    MenuStage::Render(windowDeviceContext, memoryDeviceContext, windowResolution);

    // Todo: Draw result
    {

    }
}


