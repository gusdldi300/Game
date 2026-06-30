#pragma once

#include <wtypes.h>
    
#include "Vector2.h"

class GraphicsObject
{
public:
    GraphicsObject(Vector2 position, float width, float height);
    virtual ~GraphicsObject() = default;

    virtual void Update() = 0;
    virtual void Render(HDC windowDeviceContext, HDC memoryDeviceContext, POINT windowResolution) = 0;

protected:
    Vector2 mPosition;
    float mWidth;
    float mHeight;
};

