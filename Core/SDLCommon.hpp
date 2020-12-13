#ifndef SDLCommon_hpp
#define SDLCommon_hpp

#include <cassert>

#include "SDL.h"

/// Smart lifetime containers of various structs

struct SDLSurface
{
   SDLSurface(SDL_Surface* p) : m_resource(p)
   {
      assert(p != nullptr);
   }

   ~SDLSurface()
   {
      SDL_FreeSurface(m_resource);
   }

   SDL_Surface* get() const { return m_resource; };

private:
   SDL_Surface* m_resource;
};

struct SDLTexture
{
   SDLTexture(SDL_Texture* p) : m_resource(p)
   {
      assert(p != nullptr);
   }

   ~SDLTexture()
   {
      SDL_DestroyTexture(m_resource);
   }

   SDL_Texture* get() const { return m_resource; }

private:
   SDL_Texture* m_resource;
};

/// Utils

#include "Color.hpp"

class SDLUtil
{
public:
   static SDL_Color MakeSDLColor(ColorRGB c)
   {
      uint8_t r = c >> 24;
      uint8_t g = (c >> 16) & 0x00FF;
      uint8_t b = (c >> 8) & 0x0000FF;
      return {r, g, b, 0xFF};
   }
};


#endif