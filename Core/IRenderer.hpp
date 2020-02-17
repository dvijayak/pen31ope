#ifndef IRenderer_hpp
#define IRenderer_hpp

#include "Color.hpp"

class IRenderer
{
public:    
    virtual ~IRenderer () {};

    // Fundamental routines
    virtual void SetPixel (size_t index, ColorRGB color) = 0;
	virtual void SetPixel (size_t x, size_t y, ColorRGB color) = 0;
    virtual void FillScreenBackground (ColorRGB color=Color::Black) = 0;

    // Basic drawing routines
    virtual void DrawLine (size_t x_s, size_t y_s, size_t x_e, size_t y_e, ColorRGB color);

    /**
     * Should be invoked once a frame of pixels is ready to be sent to a video device.
     */
	virtual void RenderFrame () const = 0;
};

#endif