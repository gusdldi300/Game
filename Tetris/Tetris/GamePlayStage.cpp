
#include "GamePlayStage.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include <cassert>

GamePlayStage::GamePlayStage(MainBoard* mainBoard, TetrominoManager* tetrominoManager, GameStats* gameStats)
    : mMainBoard(mainBoard)
    , mTetrominoManager(tetrominoManager)
    , mGameStats(gameStats)
{
}

void GamePlayStage::Update(double deltaTime)
{
    unsigned int clearLinesCount = mMainBoard->ClearLines(mTetrominoManager);

    mGameStats->ProcessLineClear(clearLinesCount);
    if (mGameStats->HasStageLevelUp())
    {
        // Todo: 속도 변경 등
    }
}
