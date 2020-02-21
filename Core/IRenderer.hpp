#ifndef IRenderer_hpp
#define IRenderer_hpp

#include "global.hpp"
#include "Color.hpp"

class Vector3;

class IRenderer
{
public:    
    virtual ~IRenderer () {};

    // Fundamental routines
    virtual void SetPixel (uint index, ColorRGB color) = 0;
    virtual void SetPixel (uint x, uint y, ColorRGB color) = 0;
    virtual void FillScreenBackground (ColorRGB color=Color::Black) = 0;

    // Basic drawing routines
    virtual void DrawLine (Vector3 const& from, Vector3 const& to, ColorRGB color) = 0;
    virtual void DrawTriangle (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2, ColorRGB color) = 0;

    /**
     * Should be invoked once a frame of pixels is ready to be sent to a video device.
     */
    virtual void RenderFrame () = 0;
};

#endif