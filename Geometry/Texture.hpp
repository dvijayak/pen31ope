#ifndef Texture_hpp
#define Texture_hpp

#include "global.hpp"

#include <vector>
#include <cassert>

#include "Color.hpp"
#include "Vector.hpp"

/**
 * Maps between a uv coordinate to pixel data from a texture
 */
class TextureMap
{
public:
   typedef std::vector<ColorRGB> PixelBuffer;

   uint m_width, m_height;

   /**
    * Should support 24-bit color values
    */
   PixelBuffer m_pixels;

public:
   TextureMap (uint width, uint height, PixelBuffer const& pixels)
      : m_width(width), m_height(height), m_pixels(pixels)
   {}

   ColorRGB Map (Vector2 const& uv ) const { return Map(uv.x, uv.y); }
   ColorRGB Map (float const u, float const v) const
   {
      // u,v coordinates map 0,0 to the bottom-left and 1,1 to the top-right corners of the texture,
      // but pixels are stored such that 0,0 is the bottom-left and 1,1 is the *bottom*-right, i.e.
      // the vertical axis is flipped. We account for this by flipping hte `v` coordinate.
      uint index = static_cast<uint>(u * m_width) + static_cast<uint>((1.f - v) * m_height) * m_height; 
      return m_pixels[index];
   }
};

#endif
