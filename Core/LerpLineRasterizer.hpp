#ifndef LerpLineRasterizer_hpp
#define LerpLineRasterizer_hpp

#include "Rasterizer.hpp"
#include "ILineRasterizer.hpp"

#include <cmath>
#include "Interpolate.hpp"

/**
 * Alternative lerp-based approach to line rasterization that is more concise than Bresenham's algo,
 * courtesy of https://www.redblobgames.com/grids/line-drawing.html
 */
class LerpLineRasterizer
   : virtual public Rasterizer
   , virtual public ILineRasterizer
{
public:
   virtual ~LerpLineRasterizer () {}

   LerpLineRasterizer (IRenderer* pRenderer=nullptr) : Rasterizer(pRenderer) {}

   void DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color) override;
};

void LerpLineRasterizer::DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color)
{
   int dx = x_e - x_s, dy = y_e - y_s;
   float distance = sqrtf(dx*dx + dy*dy);
   uint maxSteps = floorf(distance);

   for (uint step = 0; step <= maxSteps; ++step)
   {
      float t = maxSteps == 0 ? 0 : step / float(maxSteps);
      float x = Lerp(x_s, x_e, t);
      float y = Lerp(y_s, y_e, t);
      GetRenderer()->SetPixel(floorf(x), floorf(y), color);
   }
}

#endif
