#ifndef ITriangleRasterizer_hpp
#define ITriangleRasterizer_hpp

#include "global.hpp"
#include "Color.hpp"

class ITriangleRasterizer
{
public:
   virtual ~ITriangleRasterizer () {}

   virtual void DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color) = 0;
};

#endif
