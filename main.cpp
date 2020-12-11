#include "global.hpp"

#include <memory>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define SDL_MAIN_HANDLED
#include "SDLRenderer.hpp"

#include "Logger.hpp"
#include "Game.hpp"

#include "AppSettings.hpp"
#include "LuaAppSettingsFactory.hpp"

pen31ope::AppSettings defaultSettings = {
    "scene.lua",
    1024, 768,
    pen31ope::WindowedMode::WINDOWED
};

int main (int argc, char** argv)
{
    // INITIALIZE_BASIC_LOGGERS();

    std::string settingsFile;

    // Command-line args
    if (argc >= 2)
    {
        settingsFile = argv[1];
    }
    if (settingsFile.empty())
    {
        settingsFile = "settings.lua";
    }

    pen31ope::AppSettings settings = defaultSettings;
    {
        // Load application settings from config script
        std::unique_ptr<IAppSettingsFactory> settingsFactory = std::make_unique<LuaAppSettingsFactory>();
        auto rc = settingsFactory->ReadFromFile(settingsFile);
        if (!rc)
        {
            std::cerr << "Failed to load settings from file " << settingsFile << std::endl;
        }
        else
        {
            settings = *rc.value;
        }    
    }
    pen31ope::AppSettings::Validate(settings);

    SDL_SetMainReady();
    std::unique_ptr<SDLRenderer> pSDL = std::make_unique<SDLRenderer>(); // resources are freed at the end via RAII
    pSDL->Initialize(argv[0], settings.screenWidth, settings.screenHeight);

    // Inititalize RNGs
    // srand(time(nullptr));
    srand(0xff12ffcc);

    // Initialize and run the game
    // The block ensures that game-related resources are released as soon as the game
    // finishes running. This is done via RAII.
    int rc;
    {
        Game game;

        // Configuration
        game.SetRenderer(pSDL.get());
        game.SetScreenWidthAndHeight(settings.screenWidth, settings.screenHeight);        

        // Go!
        rc = game.Run();
    }

    // spdlog::drop_all(); 	// in Windows, this must be called before main finishes to workaround a known VS issue

    return rc;
}