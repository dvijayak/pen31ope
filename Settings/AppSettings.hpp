#ifndef AppSettings_hpp
#define AppSettings_hpp

#include <memory>
#include <string>
#include <cassert>

namespace pen31ope
{
   enum WindowedMode {
      WINDOWED,
      FULLSCREEN,
      BORDERLESS
   };

   struct AppSettings
   {
      std::string startingSceneScript = "scene.lua"; // doesn't have to be a Lua script, though
      int screenWidth = 640, screenHeight = 480;
      WindowedMode windowedMode = WindowedMode::WINDOWED;

      struct LoadResult
      {
         bool success = false;
         std::unique_ptr<AppSettings> value = nullptr;

         operator bool() const
         {
            return success;
         }
      };

      /**
       * Prefer to halt app rather than fail gracefully so that the root cause can be identified quickly during development
       */
      static bool Validate (AppSettings const& settings)
      {
         assert(settings.screenWidth > 0 && settings.screenWidth <= 3840);
         assert(settings.screenHeight > 0 && settings.screenHeight <= 3840);

         switch (settings.windowedMode)
         {
            case WINDOWED:
            case FULLSCREEN:
            case BORDERLESS:
               break;
            default:
               assert(false);
         }

         assert(!settings.startingSceneScript.empty());

         return true; // useless for now
      }
   };

}

#endif
