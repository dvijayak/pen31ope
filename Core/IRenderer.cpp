#include "IRenderer.hpp"

#include <cmath>

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

static float lerp (float v0, float v1, float t)
{
   return v0 + (v1 - v0) * t;
   // OR
   // return v0 * (1.f - t) + v1 * t;
}

void IRenderer::DrawLine (uint x0, uint y0, uint x1, uint y1, ColorRGB color)
{
   int dx = x1 - x0, dy = y1 - y0;
   float distance = sqrtf(dx*dx + dy*dy);
   uint maxSteps = floorf(distance);

   for (uint step = 0; step <= maxSteps; ++step)
   {
      float t = maxSteps == 0 ? 0 : step / float(maxSteps);
      float x = lerp(x0, x1, t);
      float y = lerp(y0, y1, t);
      SetPixel(floorf(x), floorf(y), color);
   }
}

 */
