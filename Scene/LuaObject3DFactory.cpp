#include "LuaObject3DFactory.hpp"

#include "Constants.hpp"

#include "Object3D.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

#include "SDLTextureLoader.hpp"

LuaObject3DFactory::LuaObject3DFactory ()
{
   m_pTextureLoader = std::make_unique<SDLTextureLoader>();
}

std::vector<Object3D> LuaObject3DFactory::MakeFromFile (std::string const& filename)
{
   std::vector<Object3D> objects;
      
   if (!_.LoadFromFile(filename))
      return objects;

   sol::optional<sol::table> config = _.lua["_"]["objects"];
   if (!config)
      return objects;

   auto & objectsTable = config.value();
   int length = objectsTable.size();
   for (int i = 1; i <= length; ++i)
   {
      auto element = objectsTable[i];

      Object3D object;

      sol::optional<std::string> meshStr = element["mesh"];
      if (meshStr)
      {
         // First, interpret as mesh OBJ filepath
         auto mesh = Mesh::MakeFromOBJ(meshStr.value());

         // TODO: Check if it identifies a pre-defined primitive mesh

         // TODO: For now, just ignore if no mesh was makeable
         if (!mesh) continue;

         if (mesh)
         {
            object.m_mesh = std::move(mesh);
         }
      }
      // TODO: Fallback to default mesh (cube) if could not read mesh

      auto material = element["material"];
      if (material.valid())
      {  
         // Prepare material
         std::unique_ptr<Material> pMaterial(new Material());
         bool materialIsUseful = false;

         sol::optional<std::string> diffuseStr = material["diffuse"];
         auto pDiffuseTexture = m_pTextureLoader->LoadFromFile(diffuseStr.value());
         if (pDiffuseTexture != nullptr)
         {
            materialIsUseful |= true;

            pMaterial->DiffuseMap(std::move(pDiffuseTexture));
         }

         if (materialIsUseful)
         {
            object.m_material = std::move(pMaterial);
         }
         else
         {
            std::cout << "Warning: No Material components were loaded successfully" << std::endl;
         }
      }

      // Transform (model to world)
      object.ModelMatrix(Matrix4::Identity());

      auto transform = element["transform"];
      if (transform.valid())
      {
         // TODO: Write direct converter from Lua state  stack to Vector3...cf. https://sol2.readthedocs.io/en/latest/tutorial/customization.html
         Vector3 newPosition;
         sol::optional<std::vector<float>> position = transform["position"];
         if (position)
         {
            auto & v = position.value();
            newPosition = Vector3(v[0], v[1], v[2]);
            object.Translate(newPosition);
         }

         sol::optional<std::vector<float>> rotation = transform["rotation"];
         if (rotation)
         {
            auto & v = rotation.value();
            object.Rotate(Constants::Deg2Rad(v[0]), Constants::Deg2Rad(v[1]), Constants::Deg2Rad(v[2]));
         }
      }

      objects.push_back(std::move(object));
   }

   return objects;
}