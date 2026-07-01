
#include "framework.h"
#include "GraphicsGrid.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include <string>

const float GraphicsGrid::BLOCK_LENGTH = 30.f;

GraphicsGrid::GraphicsGrid(Vector2 position, int rowSize, int colSize)
    : mPosition(position)
    , mGridRowSize(rowSize)
    , mGridColSize(colSize)
{
    mbGrid = new bool* [mGridRowSize];
    for (int row = 0; row < mGridRowSize; ++row)
    {
        mbGrid[row] = new bool[mGridColSize];

        memset(mbGrid[row], false, sizeof(bool) * mGridColSize);
    }

    // Todo: Make as unmoveable block
    // Todo: row = SPAWN_ZONE_ROW_SIZE
    for (unsigned int row = 0; row < mGridRowSize; ++row)
    {
        mbGrid[row][0] = true;
        mbGrid[row][mGridColSize - 1] = true;
    }

    for (unsigned int col = 0; col < mGridColSize; ++col)
    {
        mbGrid[mGridRowSize - 1][col] = true;
    }
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
    // Draw grid
    {
        Rectangle(memoryDeviceContext, -1, -1, windowResolution.x + 1, windowResolution.y + 1);

        for (unsigned int row = 0; row < mGridRowSize; ++row)
        {
            for (unsigned int col = 0; col < mGridColSize; ++col)
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

        /*
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
        */
    }

    // Draw strings
    /*
    {
        std::wstring printScore = L"Score: " + std::to_wstring(mTotalScore);
        std::wstring printLevel = L"Stage Level: " + std::to_wstring(mStageLevel);

        // Todo: No magic number, move position
        const float PRINT_OFFSET = 10.f;
        const float PRINT_STRING_OFFSET = 30.f;

        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET, printScore.c_str(), printScore.length());
        TextOut(memoryDeviceContext, 0, GRID_HEIGHT + PRINT_OFFSET + PRINT_STRING_OFFSET, printLevel.c_str(), printLevel.length());
    }

    BitBlt(windowDeviceContext, 0, 0, windowResolution.x, windowResolution.y,
        memoryDeviceContext, 0, 0, SRCCOPY);
    */

}