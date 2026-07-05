#include <cassert>

#include "EndStage.h"
#include "KeyManager.h"

// Todo: Use constexpr
const Vector2 EndStage::LEFT_TOP_DRAW_VECTOR = { 450.f, 250.f };

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
    : MenuStage({ 450.f, 200.f }, TITLE_STRING, MENU_STRINGS, SELECTED_MENU_STRINGS)
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

void EndStage::Render(HDC memoryDeviceContext)
{
    MenuStage::Render(memoryDeviceContext);

    Vector2 drawScoreLeftTopVector = LEFT_TOP_DRAW_VECTOR;
    drawScoreLeftTopVector.Y += (MENU_STRINGS.size() * STRING_DRAW_OFFSET) + STRING_DRAW_OFFSET;

    std::vector<std::wstring> drawStats;
    std::wstring scoreString = L"Final Score: " + std::to_wstring(mResult.Score);
    std::wstring levelString = L"Stage Level: " + std::to_wstring(mResult.Level);
    std::wstring linesClearedString = L"Lines Cleared: " + std::to_wstring(mResult.LineCleared);

    drawStats.push_back(scoreString);
    drawStats.push_back(levelString);
    drawStats.push_back(linesClearedString);

    drawScores(memoryDeviceContext, drawScoreLeftTopVector, drawStats);
}


