#include "global.hpp"

#include <memory>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define SDL_MAIN_HANDLED

#include "SDLRenderer.hpp"
#include "Game.hpp"
#include "Logger.hpp"

uint WIDTH = 1024;
uint HEIGHT = 768;

int main (int argc, char** argv)
{
    // INITIALIZE_BASIC_LOGGERS();

    // Command-line args
    if (argc >= 3)
    {
        // Set game graphics resolution
        uint width, height;

        std::stringstream ss_w(argv[1]), ss_h(argv[2]);
        ss_w >> width;
        ss_h >> height;

        if (width > 0 && width <= 3840)
        {
            WIDTH = width;
        }
        if (height > 0 && height <= 3840)
        {
            HEIGHT = height;
        }
    }

    SDL_SetMainReady();
    std::unique_ptr<SDLRenderer> pSDL = std::make_unique<SDLRenderer>(); // resources are freed at the end via RAII
    pSDL->Initialize(argv[0], WIDTH, HEIGHT);

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
        game.SetScreenWidth(WIDTH);
        game.SetScreenHeight(HEIGHT);

        // Go!
        rc = game.Run();
    }

    // spdlog::drop_all(); 	// in Windows, this must be called before main finishes to workaround a known VS issue

    return rc;
}