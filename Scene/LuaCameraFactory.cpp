#include "LuaCameraFactory.hpp"

#include "LuaVector.hpp"

#include "Camera.hpp"
#include "Constants.hpp"

std::unique_ptr<Camera> LuaCameraFactory::MakeFromFile (std::string const& filename)
{
   if (!_.LoadFromFile(filename))
      return nullptr;

   auto config = _.lua["_"]["camera"];
   if (!config.valid())
      return nullptr;

   auto camera = std::make_unique<Camera>();

   sol::optional<float> fov = config["fov"];
   if (fov)
   {
      camera->Fov(Constants::Deg2Rad(fov.value()));
   }

   sol::optional<float> near = config["near"];
   if (near)
   {
      camera->Near(near.value());
   }

   sol::optional<float> far = config["far"];
   if (far)
   {
      camera->Far(far.value());
   }

   // TODO: Implement proper transform for Camera so that its positioning + orientation works just like Object3D
   Vector3 newPosition;
   sol::optional<std::array<float, 3>> position = config["transform"]["position"];
   if (position)
   {
      auto & v = position.value();
      newPosition = Vector3(v[0], v[1], v[2]);
   }

   sol::optional<std::array<float, 3>> lookAt = config["look_at"]["at"];
   sol::optional<std::array<float, 3>> up = config["look_at"]["up"];
   if (lookAt && up)
   {
      auto & v_at = lookAt.value();
      auto & v_up = up.value();
      camera->LookAt(newPosition, Vector3(v_at[0], v_at[1], v_at[2]), Vector3(v_up[0], v_up[1], v_up[2]));
   }

   return std::move(camera);
}
