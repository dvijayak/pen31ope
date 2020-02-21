#ifndef BarycentricTriangleRasterizer_hpp
#define BarycentricTriangleRasterizer_hpp

#include "Rasterizer.hpp"
#include "ITriangleRasterizer.hpp"

#include "Vector3.hpp"
#include "Box.hpp"
#include "Triangle.hpp"

/**
 * Rasterizes pixels using barycentric coordinates relative to the triangle. 
 * I don't fully understand the theory, so this code is mostly courtesy of [ssloy](https://github.com/ssloy/tinyrenderer/wiki/Lesson-2-Triangle-rasterization-and-back-face-culling)
 * Observation: This is the fundamental rasterization approach generally used in ray-tracing algorithms.
 */
class BarycentricTriangleRasterizer
   : virtual public Rasterizer
   , virtual public ITriangleRasterizer
{
public:
   virtual ~BarycentricTriangleRasterizer () {}

   BarycentricTriangleRasterizer (IRenderer* pRenderer=nullptr) : Rasterizer(pRenderer) {}

   void DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color) override;
};

void BarycentricTriangleRasterizer::DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color)
{
   std::array<Vector3, 3> vertices = {
      Vector3(x0, y0),
      Vector3(x1, y1),
      Vector3(x2, y2)
   };

   auto const boundingBox = TriangleUtil::MinimumBoundingBox(vertices);
   
   uint x_start = boundingBox.topLeft[0], y_start = boundingBox.topLeft[1];
   uint x_end = boundingBox.bottomRight[0], y_end = boundingBox.bottomRight[1];
   for (uint x = x_start; x <= x_end; ++x)
   {
      for (uint y = y_start; y <= y_end; ++y)
      {
         Vector3 baryCoords = TriangleUtil::BarycentricCoordinates(Vector3(x, y), vertices);

         if (baryCoords.x() >= 0 && baryCoords.y() >= 0 && baryCoords.z() >= 0)
         {
            GetRenderer()->SetPixel(x, y, color);
         }
      }
   }
}

#endif
