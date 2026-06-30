
#include <cassert>

#include "Grid.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "ITetromino.h"

// Todo: Make as class static
/*
const Vector2 Grid::GRID_START_POINT =
{
    0.f, 0.f
};
*/

const float Grid::BLOCK_LENGTH = 30.f;
const float Grid::GRID_WIDTH = GRID_ROW_SIZE * BLOCK_LENGTH;
const float Grid::GRID_HEIGHT = GRID_COL_SIZE * BLOCK_LENGTH;

Grid::Grid()
    : GraphicsObject({ 0.f, 0.f }, GRID_WIDTH, GRID_HEIGHT)
    , mTetromino(nullptr) // Todo: mTetromino to nullptr
{
    // Todo: Make random tetromino
    spawnTetromino(new ITetromino({ 0, 0 }));
    for (unsigned int i = 0; i < MAX_NEXT_TETROMINOS_COUNT; ++i)
    {
        addRandomTetromino(&mNextTetrominoList);
    }

    mbGrid = new bool*[GRID_ROW_SIZE];
    for (int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        mbGrid[row] = new bool[GRID_COL_SIZE];

        memset(mbGrid[row], false, sizeof(bool) * GRID_COL_SIZE);
    }

    // Todo: Make as unmoveable block
    for (unsigned int row = SPAWN_ZONE_ROW_SIZE; row < GRID_ROW_SIZE; ++row)
    {
        mbGrid[row][0] = true;
        mbGrid[row][GRID_COL_SIZE - 1] = true;
    }

    for (unsigned int col = 0; col < GRID_COL_SIZE; ++col)
    {
        mbGrid[GRID_ROW_SIZE - 1][col] = true;
    }
}

Grid::~Grid()
{
    for (int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        delete[] mbGrid[row]; 
    }
    delete[] mbGrid;

    while (mNextTetrominoList.empty() == false)
    {
        delete mNextTetrominoList.front();
        mNextTetrominoList.pop_front();
    }

    delete mTetromino;
}

void Grid::Update()
{
    assert(mTetromino != nullptr);

    bool bTetrominoAlive = true;
    if (TimeManager::GetInstance()->HasTicked())
    {
        // Move down
        TimeManager::GetInstance()->ResetTick();

        bTetrominoAlive = mTetromino->MoveOneStep(eDirection::Down, *this);
    }
    else
    {
        // Todo: Maybe change to switch case
        KeyManager* keyManager = KeyManager::GetInstance();
        if (keyManager->GetKeyState(eKey::Left) == eKeyState::Press)
        {
            mTetromino->MoveOneStep(eDirection::Left, *this);
        }
        else if (keyManager->GetKeyState(eKey::Right) == eKeyState::Press)
        {
            mTetromino->MoveOneStep(eDirection::Right, *this);
        }
        else if (keyManager->GetKeyState(eKey::Up) == eKeyState::Press)
        {
            mTetromino->RotateCW(*this);
        }
        else if (keyManager->GetKeyState(eKey::Space) == eKeyState::Press)
        {
            while (bTetrominoAlive)
            {
                bTetrominoAlive = mTetromino->MoveOneStep(eDirection::Down, *this);
            }

            assert(bTetrominoAlive == false);
        }
    }

    if (bTetrominoAlive == false)
    {
        // Mark dead tetromino on grid 
        for (const Position& blockPosition : mTetromino->GetBlockPositions())
        {
            mbGrid[blockPosition.GetRow()][blockPosition.GetCol()] = true;
        }
        
        // Todo: Use memory pool
        delete mTetromino;
        mTetromino = nullptr;

        // Respawn tetromino
        spawnTetromino(mNextTetrominoList.front());

        mNextTetrominoList.pop_front();
        addRandomTetromino(&mNextTetrominoList);
    }
    
}

// Todo: HDC 인자 중 하나는 전달할 필요 없음
void Grid::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    // Todo: 사각형 크기 수정 필요함, 
    // Todo: 전체 도화지가 지워지기 때문에, 높은 레벨로 올려야함
    Rectangle(memoryDeviceContext, -1, -1, windowResolution.x + 1, windowResolution.y + 1);

    for (unsigned int row = 0; row < GRID_ROW_SIZE; ++row)
    {
        for (unsigned int col = 0; col < GRID_COL_SIZE; ++col)
        {
            if (mbGrid[row][col] == false)
            {
                continue;
            }

            int renderStartY = row * BLOCK_LENGTH;
            int renderStartX = col * BLOCK_LENGTH;

            Rectangle(memoryDeviceContext,
                renderStartX,
                renderStartY,
                renderStartX + BLOCK_LENGTH,
                renderStartY + BLOCK_LENGTH);
        }
    }

    for (const Position& blockPosition : mTetromino->GetBlockPositions())
    {
        int renderStartY = blockPosition.GetRow() * BLOCK_LENGTH;
        int renderStartX = blockPosition.GetCol() * BLOCK_LENGTH;

        Rectangle(memoryDeviceContext,
            renderStartX,
            renderStartY,
            renderStartX + BLOCK_LENGTH,
            renderStartY + BLOCK_LENGTH);
    }

    BitBlt(windowDeviceContext, 0, 0, windowResolution.x, windowResolution.y,
        memoryDeviceContext, 0, 0, SRCCOPY);
}

void Grid::spawnTetromino(Tetromino* tetromino)
{
    assert(tetromino != nullptr);
    assert(mTetromino == nullptr);

    mTetromino = tetromino;

    // Todo: Magic number
    mTetromino->ResetStates();
    mTetromino->MovePosition({ SPAWN_TETROMINO_ROW, SPAWN_TETROMINO_COL });
}

void Grid::addRandomTetromino(std::list<Tetromino*>* outNextTetrominoList)
{
    assert(outNextTetrominoList->size() < MAX_NEXT_TETROMINOS_COUNT);

    // Todo: Make random Tetromino
    outNextTetrominoList->push_back(new ITetromino({ 0, 0 }));
}

const bool* const* Grid::GetGrid() const
{
    return mbGrid;
}

