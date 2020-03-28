#include "Object3DFactory.hpp"

#include <cstring>

#include "SDLTextureLoader.hpp"
#include "Util.hpp"

uint Object3DFactory::s_appWideNextAvailableObjectId = 1;

Object3DFactory::Object3DFactory ()
{
   m_pTextureLoader = std::make_unique<SDLTextureLoader>();
}

Object3D* Object3DFactory::MakeTexturedObject (std::string const& objFileName, std::string const& diffuseTextureFilename)
{
   std::cout << "Making object: " << objFileName << std::endl;

   std::unique_ptr<Object3D> pObject(new Object3D());

   // Load mesh
   pObject->m_mesh = std::move(Mesh::MakeFromOBJ(objFileName));
   if (pObject->m_mesh == nullptr)
   {
      return nullptr;
   }

   // Prepare material
   bool materialIsUseful = false; // was at least one component of the material successfully loaded?
   std::unique_ptr<Material> pMaterial(new Material());

   // Load textures
   std::cout << "Diffuse Texture: " << diffuseTextureFilename << std::endl;
   auto pDiffuseTexture = m_pTextureLoader->LoadFromFile(diffuseTextureFilename);
   if (pDiffuseTexture != nullptr)
   {
      materialIsUseful |= true;

      pMaterial->m_diffuseMap = std::move(pDiffuseTexture);
   }

   if (materialIsUseful)
   {
      pObject->m_material = std::move(pMaterial);
   }
   else
   {
      std::cout << "Warning: No Material components were loaded successfully" << std::endl;
   }   

   // Initialize model matrix
   pObject->ModelMatrix(Matrix4::Identity());

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