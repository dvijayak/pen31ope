#ifndef ILineRasterizer_hpp
#define ILineRasterizer_hpp

#include "global.hpp"
#include "Color.hpp"

class ILineRasterizer
{
public:
   virtual ~ILineRasterizer () {}

   virtual void DrawLine (Vector3 const& from, Vector3 const& to, ColorRGB color) = 0;
};

#endif
