#ifndef LerpTriangleRasterizer_hpp
#define LerpTriangleRasterizer_hpp

#include "Rasterizer.hpp"
#include "ITriangleRasterizer.hpp"
#include "ILineRasterizer.hpp"

#include <cmath>
#include <array>
#include <map>
#include <algorithm>
#include <functional>

#include "Interpolate.hpp"
#include "Vector3.hpp"

class LerpTriangleRasterizer
   : virtual public Rasterizer
   , virtual public ITriangleRasterizer
{
   ILineRasterizer* m_pLineRasterizer;

public:
   virtual ~LerpTriangleRasterizer () {}

   LerpTriangleRasterizer (IRenderer* pRenderer=nullptr, ILineRasterizer* pLineRasterizer=nullptr)
      : Rasterizer(pRenderer)
      , m_pLineRasterizer(pLineRasterizer)
   {}

   void DrawTriangle (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2, ColorRGB color) override;
};

void LerpTriangleRasterizer::DrawTriangle (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2, ColorRGB color)
{
   std::array<Vector3, 3> vertices {v0, v1, v2};

   // Sort vertices by y-component in ascending order (since y increases downwards) and by x-component in ascending order
   std::sort(vertices.begin(), vertices.end(), [](auto const& a, auto const& b) {
      return int(a.y()) == int(b.y()) ? (a.x() < b.x()) : (a.y() < b.y());
   });

   // For convenience, we assume the following edge labeling scheme:
   //    AB = vertices[0], vertices[1]
   //    BC = vertices[1], vertices[2]
   //    AC = vertices[0], vertices[2]
   uint x_A = vertices[0].x(), y_A = vertices[0].y();
   uint x_B = vertices[1].x(), y_B = vertices[1].y();
   uint x_C = vertices[2].x(), y_C = vertices[2].y();

   float dy_AB = float(y_B - y_A);
   float dy_BC = float(y_C - y_B);
   float dy_AC = float(y_C - y_A);
   
   // Handle case where A and B have the same y-component
   bool hasFinishedAB = y_B - y_A == 0;

   // Linearly interpolate every row within the triangle
   for (uint y = y_A; y <= y_C; ++y)
   {
      // Edges AB and BC, starting with AB
      float t, x_t;
      if (!(hasFinishedAB = hasFinishedAB || y > y_B))
      {
         t = dy_AB == 0 ? 0 : (y - y_A) / dy_AB;
         x_t = Lerp(x_A, x_B, t);
      }
      else
      {
         t = dy_BC == 0 ? 0 : (y - y_B) / dy_BC;
         x_t = Lerp(x_B, x_C, t);
      }

      // Finally, the easiest edge: AC
      float u = dy_AC == 0 ? 1.f : (y - y_A) / dy_AC;
      float x_u = Lerp(x_A, x_C, u);

      // Draw row
      m_pLineRasterizer->DrawLine(Vector3(x_t, y), Vector3(x_u, y), color);
   }
}


#endif
