#include "SDLTextureLoader.hpp"

#include "SDL.h"
#include "SDL_image.h"
#include "Color.hpp"

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
         Uint32 color = reinterpret_cast<Uint32*>(pTextureImage->pixels)[i];
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