#include "Object3DFactory.hpp"

#include "SDL_image.h"

#include "Color.hpp"

#include <cstring>

uint Object3DFactory::s_appWideNextAvailableObjectId = 1;

Object3D* Object3DFactory::MakeTexturedObject (std::string const& objFileName, std::string const& diffuseTextureFilename)
{
   std::cout << "Making object: " << objFileName << std::endl;
   std::cout << "Diffuse Texture: " << diffuseTextureFilename << std::endl;

   std::unique_ptr<Object3D> pObject(new Object3D());

   // Load mesh
   pObject->m_mesh = std::move(Mesh::MakeFromOBJ(objFileName));
   if (pObject->m_mesh == nullptr)
   {
      return nullptr;
   }

   // Load textures
   SDL_Surface* pDiffuseTextureImage = IMG_Load(diffuseTextureFilename.c_str());
   if (!pDiffuseTextureImage)
   {
      std::cerr << "Error loading texture " << diffuseTextureFilename << " due to: " << IMG_GetError() << std::endl;
      std::cerr << "Proceeding without diffuse texture" << std::endl;
   }
   else
   {
      std::cout << "Original pixel format: " << SDL_GetPixelFormatName(pDiffuseTextureImage->format->format) << std::endl;
      constexpr Uint32 targetPixelFormat = SDL_PIXELFORMAT_RGBA8888; // pixel format MUST match that which is set in SDLRenderer.cpp for the main texture, for convenience
      std::cout << "Target pixel format: " << SDL_GetPixelFormatName(targetPixelFormat) << std::endl;

      // First, convert to our desired pixel format which is straight up RGBA
      SDL_Surface* pConvertedImage = SDL_ConvertSurfaceFormat(pDiffuseTextureImage, targetPixelFormat, 0); // last arg (flags) unused, according to SDL docs
      if (pConvertedImage == nullptr)
      {
         std::cerr << "Failed to convert diffuse image to desired pixel format; SDL error: " << SDL_GetError() << std::endl;
      }
      else
      {
         // Now we can create the texture data structure and extract the raw pixels
         TextureData textureData;
         textureData.width = pDiffuseTextureImage->w;
         textureData.height = pDiffuseTextureImage->h;
         textureData.pixels = std::vector<uint>(textureData.width * textureData.height);
         *(textureData.pixels.data()) = *(reinterpret_cast<uint*>(pConvertedImage->pixels));
         pObject->m_diffuseTextureData = textureData;

         SDL_FreeSurface(pConvertedImage);
      }

      SDL_FreeSurface(pDiffuseTextureImage);
   }

   // Store in factory and return raw pointer
   // BEGIN THREAD UNSAFE
   auto id = s_appWideNextAvailableObjectId++;
   m_objectMap[id] = std::move(pObject); // guaranteed to have no clashing IDs, so no need to check
   return m_objectMap[id].get();
   // END THREAD UNSAFE
}

Object3D* Object3DFactory::operator[] (uint id) const
{
   auto it = m_objectMap.find(id);
   if (it == m_objectMap.end())
   {
      return nullptr;
   }
   else
   {
      return it->second.get();
   }
}