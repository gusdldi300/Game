
#include "framework.h"
#include "GraphicsGrid.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include <string>

const float GraphicsGrid::BLOCK_LENGTH = 30.f;
GraphicsGrid::GraphicsGrid(Vector2 leftTopPosition, int rowSize, int colSize)
    : mLeftTopPosition(leftTopPosition)
    , mGridRowSize(rowSize)
    , mGridColSize(colSize)
{
    mbGrid = new bool* [mGridRowSize];
    for (int row = 0; row < mGridRowSize; ++row)
    {
        mbGrid[row] = new bool[mGridColSize];

        memset(mbGrid[row], false, sizeof(bool) * mGridColSize);
    }

    // Todo: row = SPAWN_ZONE_ROW_SIZE
    for (unsigned int row = 0; row < mGridRowSize; ++row)
    {
        mbGrid[row][0] = true;
        mbGrid[row][mGridColSize - 1] = true;
    }

    for (unsigned int col = 0; col < mGridColSize; ++col)
    {
        mbGrid[0][col] = true;
        mbGrid[mGridRowSize - 1][col] = true;
    }
}

GraphicsGrid::~GraphicsGrid()
{
    for (int row = 0; row < mGridRowSize; ++row)
    {
        delete[] mbGrid[row];
    }
    delete[] mbGrid;
}

//void GraphicsObject::Update()
//{
    //float frameDeltaTime = static_cast<float>(TimeManager::GetInstance()->GetFrameDeltaTime());
    //float frameDeltaTime = 10.f;
    //const float MOVE_DISTANCE_PER_SECOND = 10.f;

    //KeyManager* keyManager = KeyManager::GetInstance();

    // Todo: Move to GraphicsObject
    //if (keyManager->GetKeyState(eKey::Left) == eKeyState::Press)
    //{
    //    mPosition.X -= (MOVE_DISTANCE_PER_SECOND * frameDeltaTime);
    //}

    //if (keyManager->GetKeyState(eKey::Right) == eKeyState::Press)
    //{
    //    mPosition.X += (MOVE_DISTANCE_PER_SECOND * frameDeltaTime);
    //}
//}


// Todo: HDC 인자 중 하나는 전달할 필요 없음
void GraphicsGrid::Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution)
{
    //Rectangle(memoryDeviceContext, -1, -1, windowResolution.x + 1, windowResolution.y + 1);

    // Draw grid
    for (unsigned int row = 0; row < mGridRowSize; ++row)
    {
        for (unsigned int col = 0; col < mGridColSize; ++col)
        {
            if (mbGrid[row][col] == false)
            {
                continue;
            }

            int renderStartY = mLeftTopPosition.Y + (row * BLOCK_LENGTH);
            int renderStartX = mLeftTopPosition.X + (col * BLOCK_LENGTH);

            Rectangle(memoryDeviceContext,
                renderStartX,
                renderStartY,
                renderStartX + BLOCK_LENGTH,
                renderStartY + BLOCK_LENGTH);
        }
    }
}