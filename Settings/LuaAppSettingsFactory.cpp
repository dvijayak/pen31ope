#include "LuaAppSettingsFactory.hpp"

#include "SDL.h"

LuaAppSettingsFactory::LuaAppSettingsFactory ()
{}

pen31ope::AppSettings::LoadResult LuaAppSettingsFactory::ReadFromFile (std::string const& fileName)
{
   pen31ope::AppSettings::LoadResult rc;   

   if (!_.LoadFromFile(fileName))
      return rc;

   auto config = _.lua["_"];
   if (!config.valid())
      return rc;

   std::unique_ptr<pen31ope::AppSettings> settings = std::make_unique<pen31ope::AppSettings>();

   auto screen = config["screen"];
   if (screen.valid())
   {
      sol::optional<int> width = screen["width"];
      sol::optional<int> height = screen["height"];
      if (width && height)
      {
         settings->screenWidth = width.value();
         settings->screenHeight = height.value();
      }

      sol::optional<int> windowMode = screen["window_mode"];
      if (windowMode)
      {
         settings->windowedMode = static_cast<pen31ope::WindowedMode>(windowMode.value());
      }
   }

   sol::optional<std::string> firstScene = config["start_scene"];
   if (firstScene)
   {
      settings->startingSceneScript = firstScene.value();
   }

   rc.success = true;
   rc.value = std::move(settings);

   return rc;
}