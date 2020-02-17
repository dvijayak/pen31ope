#include "global.hpp"

#include <memory>

#define SDL_MAIN_HANDLED

#include "SDLRenderer.hpp"
#include "Game.hpp"
#include "Logger.hpp"

const uint16_t WIDTH = 1024;
const uint16_t HEIGHT = 768;

int main (int argc, char** argv)
{
    // INITIALIZE_BASIC_LOGGERS();

    SDL_SetMainReady();
    std::unique_ptr<SDLRenderer> pSDL = std::make_unique<SDLRenderer>(); // resources are freed at the end via RAII
    pSDL->Initialize("3D Library (TOBENAMED)", WIDTH, HEIGHT);

    // Initialize and run the game
    // The block ensures that game-related resources are released as soon as the game
    // finishes running. This is done via RAII.
    int rc;
    {
        Game game;

        // Configuration
        game.SetRenderer(pSDL.get());
        game.SetScreenWidth(WIDTH);
        game.SetScreenHeight(HEIGHT);

        // Go!
        rc = game.Run();
    }

    // spdlog::drop_all(); 	// in Windows, this must be called before main finishes to workaround a known VS issue

    return rc;
}