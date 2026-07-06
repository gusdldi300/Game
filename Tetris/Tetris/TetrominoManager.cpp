
#include <cassert>
#include <random>

#include "TetrominoManager.h"
#include "Vector2.h"

TetrominoManager::TetrominoManager()
    : mHoldTetrominoOrNull(nullptr)
    , mbHoldUsed(false)
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

bool TetrominoManager::HasUsedHold() const
{
    return mbHoldUsed;
}

bool TetrominoManager::HasHoldTetromino() const
{
    return mHoldTetrominoOrNull != nullptr ? true : false;
}

void TetrominoManager::SetHoldTetromino(Tetromino* tetromino)
{
    assert(tetromino != nullptr);
    
    // Todo: Set and release?
    mHoldTetrominoOrNull = tetromino;
    mHoldTetrominoOrNull->ResetStates();
}

void TetrominoManager::ResetHold()
{
    if (mHoldTetrominoOrNull != nullptr)
    {
        Release(mHoldTetrominoOrNull);
    }

    mbHoldUsed = false;
}

void TetrominoManager::Reset()
{
    ResetHold();
}

void TetrominoManager::Release(Tetromino* tetromino)
{
    assert(tetromino != nullptr);
    
    tetromino->ResetStates();
    mNextTetrominoList.push_back(tetromino);
}

const std::list<Tetromino*> TetrominoManager::GetNextTetrominoList() const
{
    return mNextTetrominoList;
}

const Tetromino* TetrominoManager::GetHoldTetrominoOrNull() const
{
    return mHoldTetrominoOrNull;
}

Tetromino* TetrominoManager::ProvideHoldTetromino()
{
    assert(mHoldTetrominoOrNull != nullptr);

    Tetromino* provideTetromino = mHoldTetrominoOrNull;
    mHoldTetrominoOrNull = nullptr;

    mbHoldUsed = true;

    return provideTetromino;
}

Tetromino* TetrominoManager::ProvideNextTetromino()
{
    Tetromino* outTetromino = mNextTetrominoList.front();
    mNextTetrominoList.pop_front();

    return outTetromino;
}

