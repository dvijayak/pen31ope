#ifndef IRenderer_hpp
#define IRenderer_hpp

#include "Color.hpp"

class IRenderer
{
public:    
    virtual ~IRenderer () {};

    virtual void SetPixel (size_t index, ColorRGB color) = 0;
	virtual void SetPixel (size_t x, size_t y, ColorRGB color) = 0;

	virtual void RenderFrame () const = 0;
};

#endif