#ifndef ILineRasterizer_hpp
#define ILineRasterizer_hpp

#include "global.hpp"
#include "Color.hpp"

class ILineRasterizer
{
public:
   virtual ~ILineRasterizer () {}

   virtual void DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color) = 0;
};

#endif
