#pragma once

#include <wtypes.h>
    
#include "Vector2.h"

class GraphicsGrid
{
public:
    GraphicsGrid(Vector2 leftTopPosition, int rowSize, int colSize);
    virtual ~GraphicsGrid() = default;

    //virtual void Update() = 0;
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution);

public:
    static const unsigned int WALL_COL_SIZE = 2U;
    static const unsigned int WALL_ROW_SIZE = 2U;

protected:
    static const float BLOCK_LENGTH;

    Vector2 mLeftTopPosition;
    //float mWidth;
    //float mHeight;

    int mGridRowSize;
    int mGridColSize;

    bool** mbGrid;
};

