#pragma once

#include <wtypes.h>
    
#include "Vector2.h"

class GraphicsGrid
{
public:
    GraphicsGrid(Vector2 position, int rowSize, int colSize);
    virtual ~GraphicsGrid() = default;

    //virtual void Update() = 0;
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) = 0;

protected:
    static const float BLOCK_LENGTH;

    Vector2 mPosition;
    //float mWidth;
    //float mHeight;

    int mGridRowSize;
    int mGridColSize;

    bool** mbGrid;
};

