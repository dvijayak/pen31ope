#include "SDLTextureLoader.hpp"

#include <cassert>

#include "SDL.h"
#include "SDL_image.h"
#include "Color.hpp"

// Courtesy of http://sdl.beuc.net/sdl.wiki/Pixel_Access
Uint32 GetRawPixelFromSurface (SDL_Surface * surface, int const index)
{
   /*
    * This algorithm takes into account the fact that image data in a surface might not always be described
    * using 4 bytes per pixel. (This can actually vary across platforms for the exact same image file! For example,
    * on my macbook pro the same file was described with 4 bytes per pixel whereas on my Windows 7 desktop it was
    * 3 bytes per pixel).
    */

   int bpp = surface->format->BytesPerPixel;
   
   // The smallest possible bpp is 1 byte, so we start with a byte-pointer to the pixel and handle all the valid cases thereafter
   Uint8 * p = (Uint8 *)surface->pixels + index * bpp;

   switch (bpp) {
      case 1:
         return *p;
         break;

      case 2:
         return *(Uint16 *)p;
         break;

      case 3:
         if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
         else
            return p[0] | p[1] << 8 | p[2] << 16;
         break;

      case 4:
         return *(Uint32 *)p;
         break;

      default:
         // We don't support any other valid (> 4) or invalid (< 1) bytes per pixel
         assert(false);
         return 0;
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
         Uint32 color = GetRawPixelFromSurface(pTextureImage, i);
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