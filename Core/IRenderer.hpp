#ifndef IRenderer_hpp
#define IRenderer_hpp

#include "global.hpp"
#include "Color.hpp"

class IRenderer
{
public:    
    virtual ~IRenderer () {};

    // Fundamental routines
    virtual void SetPixel (uint index, ColorRGB color) = 0;
	virtual void SetPixel (uint x, uint y, ColorRGB color) = 0;
    virtual void FillScreenBackground (ColorRGB color=Color::Black) = 0;

    // Basic drawing routines
    virtual void DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color);
    virtual void DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color);

    /**
     * Should be invoked once a frame of pixels is ready to be sent to a video device.
     */
	virtual void RenderFrame () = 0;
};

#endif