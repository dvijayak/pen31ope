#ifndef ITriangleRasterizer_hpp
#define ITriangleRasterizer_hpp

#include "global.hpp"
#include "Color.hpp"

class ITriangleRasterizer
{
public:
   virtual ~ITriangleRasterizer () {}

   virtual void DrawTriangle (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2, ColorRGB color) = 0;
};

#endif
