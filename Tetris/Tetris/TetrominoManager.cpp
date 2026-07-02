
#include <cassert>
#include <random>

#include "Tetromino.h"
#include "TetrominoManager.h"

class Vector2;

const unsigned int TetrominoManager::MAX_NEXT_TETROMINOS_COUNT = 40U;
const unsigned int TetrominoManager::MAX_NEXT_TETROMINOS_SHOW_COUNT = 5U;

TetrominoManager::TetrominoManager(Vector2 leftTopPosition)
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
    assert(tetromino != nullptr);
    
    tetromino->ResetStates();
    mNextTetrominoList.push_back(tetromino);
}

const std::list<Tetromino*> TetrominoManager::GetNextTetrominoList() const
{
    return mNextTetrominoList;
}

Tetromino* TetrominoManager::GetNextTetromino()
{
    Tetromino* outTetromino = mNextTetrominoList.front();
    mNextTetrominoList.pop_front();

    return outTetromino;
}

