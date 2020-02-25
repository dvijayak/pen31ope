#include "Object3DFactory.hpp"

#include "SDL_image.h"

#include "Color.hpp"

#include <cstring>

#include "Util.hpp"

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

   // TODO: Tight dependency with SDL library. Should abstract the SDL part out
   // Load textures
   SDL_Surface* pDiffuseTextureImage = IMG_Load(diffuseTextureFilename.c_str());
   if (!pDiffuseTextureImage)
   {
      std::cerr << "Error loading texture " << diffuseTextureFilename << " due to: " << IMG_GetError() << std::endl;
      std::cerr << "Proceeding without diffuse texture" << std::endl;
   }
   else
   {
      TextureData textureData;
      textureData.width = pDiffuseTextureImage->w;
      textureData.height = pDiffuseTextureImage->h;

      // Extract raw pixel data
      size_t totalPixels = textureData.width * textureData.height;
      textureData.pixels = std::vector<uint>(totalPixels);      
      for (size_t i = 0; i < totalPixels; ++i)
      {
         Uint32 color = reinterpret_cast<Uint32*>(pDiffuseTextureImage->pixels)[i];
         Uint8 r, g, b;
         SDL_GetRGB(color, pDiffuseTextureImage->format, &r, &g, &b);
         ColorRGB finalColor = Color::Mix(r, g, b);
         textureData.pixels[i] = finalColor;
      }

      // Texture data preparation complete
      pObject->m_diffuseTextureData = std::move(textureData);

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