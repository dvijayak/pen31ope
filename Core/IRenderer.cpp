#include "IRenderer.hpp"

#include <cmath>

void IRenderer::DrawLine (size_t x_s, size_t y_s, size_t x_e, size_t y_e, ColorRGB color)
{
   int dx = x_e - x_s;
   int dy = y_e - y_s;   

   if (dx > dy)
   {
      for (size_t x = x_s; x <= x_e; x++)
      {
         float t = float(x - x_s)/float(x_e - x_s);
         float y = y_s + (dy * t);
         SetPixel(x, size_t(std::roundf(y)), color);
      }
   }
   else
   {
      for (size_t y = y_s; y <= y_e; y++)
      {
         float t = float(y - y_s)/float(y_e - y_s);
         float x = x_s + (dx * t);
         SetPixel(size_t(std::roundf(x)), y, color);
      }
   }
}