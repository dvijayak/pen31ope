#include "SDLTextureLoader.hpp"

#include <cassert>

#include "SDL.h"
#include "SDL_image.h"
#include "Color.hpp"

// Courtesy of http://sdl.beuc.net/sdl.wiki/Pixel_Access
Uint32 getpixel (SDL_Surface *surface, int i)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + i * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        assert(false);
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

std::unique_ptr<TextureMap> SDLTextureLoader::LoadFromFile (std::string fileName)
{
   std::cout << "Loading texture from " << fileName << std::endl;

   // Load textures
   SDL_Surface* pTextureImage = IMG_Load(fileName.c_str());
   if (pTextureImage == nullptr)
   {
      std::cerr << "Error loading texture " << fileName << " due to: " << IMG_GetError() << std::endl;
      return nullptr;
   }
   else
   {
      // Extract raw pixel data
      size_t totalPixels = pTextureImage->w * pTextureImage->h;
      auto pixels = TextureMap::PixelBuffer(totalPixels);
      for (size_t i = 0; i < totalPixels; ++i)
      {
         Uint32 color = getpixel(pTextureImage, i);
         Uint8 r, g, b;
         SDL_GetRGB(color, pTextureImage->format, &r, &g, &b);
         ColorRGB finalColor = Color::Mix(r, g, b);
         pixels[i] = finalColor;
      }

      // Construct texture map
      std::unique_ptr<TextureMap> pTexture(new TextureMap(pTextureImage->w, pTextureImage->h, std::move(pixels)));

      // Destroy SDL data before proceeding
      SDL_FreeSurface(pTextureImage);

      return std::move(pTexture);
   }
}