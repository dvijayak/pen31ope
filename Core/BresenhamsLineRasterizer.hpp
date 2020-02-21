#ifndef BresenhamsLineRasterizer_hpp
#define BresenhamsLineRasterizer_hpp

#include "Rasterizer.hpp"
#include "ILineRasterizer.hpp"

#include <cmath>

class BresenhamsLineRasterizer
   : virtual public Rasterizer
   , virtual public ILineRasterizer
{
public:
   virtual ~BresenhamsLineRasterizer () {}

   BresenhamsLineRasterizer (IRenderer* pRenderer=nullptr) : Rasterizer(pRenderer) {}

   void DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color) override;
};

void BresenhamsLineRasterizer::DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color)
{
   int dx = x_e - x_s; // using a signed int is critical
   int dy = y_e - y_s;

   // Handle degenerate case when a "line" is really just a single point
   // in order to avoid division by zero later
   if (dx == 0 && dy == 0)
   {
      GetRenderer()->SetPixel(x_s, y_s, color);
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
         GetRenderer()->SetPixel(y, x, color);
      }
      else
      {
         GetRenderer()->SetPixel(x, y, color);
      }
   }
}

#endif
