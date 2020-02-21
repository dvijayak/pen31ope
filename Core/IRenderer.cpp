#include "IRenderer.hpp"

#include <cmath>
#include <array>
#include <map>
#include <algorithm>
#include <functional>

#include "Interpolate.hpp"
#include "Vector3.hpp"
#include "Box.hpp"
#include "Triangle.hpp"

void IRenderer::DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color)
{
   int dx = x_e - x_s; // using a signed int is critical
   int dy = y_e - y_s;

   // Handle degenerate case when a "line" is really just a single point
   // in order to avoid division by zero later
   if (dx == 0 && dy == 0)
   {
      SetPixel(x_s, y_s, color);
      return;
   }

   bool steep = std::abs(dx) < std::abs(dy); // is |y| growing faster than |x|?

   // Transpose the line, i.e. project the line onto the inverse octant
   if (steep)
   {
      std::swap(x_s, y_s);
      std::swap(x_e, y_e);
   }

   // At this point, the rest of the function can assume that |x| is always growing faster than |y|

   // Handle right-to-left lines by turning them into left-to-right
   if (x_e < x_s)
   {
      std::swap(x_s, x_e);
      std::swap(y_s, y_e);
   }

   // By now, we are guaranteed that the line always starts from the left and ends in the right, and has 0 <= slope <= 1 (i.e. run is always greater than rise AND run is always positive)

   int dxFinal = x_e - x_s;
   int dyFinal = y_e - y_s;
   for (uint x = x_s; x <= x_e; x++)
   {
      float t = (x - x_s) / float(dxFinal);
      uint y = static_cast<uint>(floorf(y_s + float(dyFinal * t)));

      // If we had transposed the line, transpose it again to return it to its original octant
      if (steep)
      {
         SetPixel(y, x, color);
      }
      else
      {
         SetPixel(x, y, color);
      }
   }
}

/**
 * Alternative lerp approach that is more concise than Bresenham's algo,
 * courtesy of https://www.redblobgames.com/grids/line-drawing.html

void IRenderer::DrawLine (uint x0, uint y0, uint x1, uint y1, ColorRGB color)
{
   int dx = x1 - x0, dy = y1 - y0;
   float distance = sqrtf(dx*dx + dy*dy);
   uint maxSteps = floorf(distance);

   for (uint step = 0; step <= maxSteps; ++step)
   {
      float t = maxSteps == 0 ? 0 : step / float(maxSteps);
      float x = Lerp(x0, x1, t);
      float y = Lerp(y0, y1, t);
      SetPixel(floorf(x), floorf(y), color);
   }
}

 */

void IRenderer::DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color)
{
   std::array<std::pair<uint, uint>, 3> vertices {
      std::make_pair(x0, y0),
      std::make_pair(x1, y1),
      std::make_pair(x2, y2)
   };

   // Sort vertices by y-component in ascending order (since y increases downwards) and by x-component in ascending order
   std::sort(vertices.begin(), vertices.end(), [](auto const& a, auto const& b) {
      float x_a = a.first, y_a = a.second;
      float x_b = b.first, y_b = b.second;
      return (y_a == y_b) ? (x_a < x_b) : (y_a < y_b);
   });

   // For convenience, we assume the following edge labeling scheme:
   //    AB = vertices[0], vertices[1]
   //    BC = vertices[1], vertices[2]
   //    AC = vertices[0], vertices[2]
   uint x_A = vertices[0].first, y_A = vertices[0].second;
   uint x_B = vertices[1].first, y_B = vertices[1].second;
   uint x_C = vertices[2].first, y_C = vertices[2].second;

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
      DrawLine(static_cast<uint>(x_t), y, static_cast<uint>(x_u), y, color);
   }
}

/**
 * Alternative approach using barycentric coordinates relative to the triangle.
 * I don't fully understand the theory, so this code is mostly courtesy of [ssloy](https://github.com/ssloy/tinyrenderer/wiki/Lesson-2-Triangle-rasterization-and-back-face-culling)
 * This is the rasterization approach used in ray-tracing algorithms

void IRenderer::DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color)
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
               SetPixel(x, y, color);
         }
      }
   }
}

 */
