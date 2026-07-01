
#include <random>

#include "Tetromino.h"
#include "TetrominoManager.h"

const unsigned int TetrominoManager::MAX_NEXT_TETROMINOS_COUNT = 50U;

TetrominoManager::TetrominoManager()
    : mMainTetromino(new Tetromino(eTetrominoType::L))
    , mHoldTetromino(new Tetromino(eTetrominoType::I))
    , mStageLevel(1)
    , mTotalScore(0)
{
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<unsigned int> dis(0, Tetromino::TYPES_COUNT - 1);

    for (unsigned int i = 0; i < MAX_NEXT_TETROMINOS_COUNT; ++i)
    {
        unsigned int randomNumber = dis(gen);
        eTetrominoType type = static_cast<eTetrominoType>(randomNumber);

        mNextTetrominoList.push_back(new Tetromino(type));
    }
}

TetrominoManager::~TetrominoManager()
{
    while (mNextTetrominoList.empty() == false)
    {
        delete mNextTetrominoList.front();
        mNextTetrominoList.pop_front();
    }
}

void TetrominoManager::Release(Tetromino* tetromino)
{
    tetromino->ResetStates();
    mNextTetrominoList.push_back(tetromino);
}

Tetromino* TetrominoManager::GetNextTetromino()
{
    Tetromino* outTetromino = mNextTetrominoList.front();
    mNextTetrominoList.pop_front();

    return outTetromino;
}

