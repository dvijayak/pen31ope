#ifndef BarycentricTriangleRasterizer_hpp
#define BarycentricTriangleRasterizer_hpp

#include "Rasterizer.hpp"
#include "ITriangleRasterizer.hpp"

#include <vector>

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
   typedef std::vector<float> ZBuffer_type;
   ZBuffer_type m_zBuffer;
   uint m_zBufferWidth = 0;

public:
   virtual ~BarycentricTriangleRasterizer () {}

   BarycentricTriangleRasterizer (IRenderer* pRenderer=nullptr) : Rasterizer(pRenderer) {}

   void UpdateScreenResolution (uint const width, uint const height);

   void DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color) override;
   void DrawTriangleZBuffer (Vector3 const& v1, Vector3 const& v2, Vector3 const& v3, ColorRGB color) override;
};

void BarycentricTriangleRasterizer::UpdateScreenResolution (uint const width, uint const height)
{
   m_zBuffer = ZBuffer_type(width * height, std::numeric_limits<ZBuffer_type::value_type>::lowest()); // don't use `min()`, as it doesn't work as expected for floating-point type; cf. https://en.cppreference.com/w/cpp/types/numeric_limits/lowest
   m_zBufferWidth = width;
}

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

         float u = baryCoords.x(), v = baryCoords.y(), w = baryCoords.z();
         if (u >= 0 && v >= 0 && w >= 0)
         {
            GetRenderer()->SetPixel(x, y, color);                        
         }
      }
   }
}

// TODO: I don't like the duplication here...perhaps we can consolidate this and DrawTriangle into one function
void BarycentricTriangleRasterizer::DrawTriangleZBuffer (Vector3 const& v1, Vector3 const& v2, Vector3 const& v3, ColorRGB color)
{
   std::array<Vector3, 3> vertices = {v1, v2, v3};

   auto const boundingBox = TriangleUtil::MinimumBoundingBox(vertices);
   
   uint x_start = boundingBox.topLeft[0], y_start = boundingBox.topLeft[1];
   uint x_end = boundingBox.bottomRight[0], y_end = boundingBox.bottomRight[1];
   for (uint x = x_start; x <= x_end; ++x)
   {
      for (uint y = y_start; y <= y_end; ++y)
      {
         Vector3 baryCoords = TriangleUtil::BarycentricCoordinates(Vector3(x, y), vertices);

         float u = baryCoords.x(), v = baryCoords.y(), w = baryCoords.z();
         if (u >= 0 && v >= 0 && w >= 0)
         {
            if (m_zBuffer.empty())
            {
               GetRenderer()->SetPixel(x, y, color);
            }
            else
            {
               float z = u * v1.z() + v * v2.z() + w * v3.z();
               uint index = y * m_zBufferWidth + x;
               if (z >= m_zBuffer[index])
               {
                  m_zBuffer[index] = z;
                  GetRenderer()->SetPixel(x, y, color);
               }
            }   
         }
      }
   }
}

#endif
