#include "Game.hpp"

#include "global.hpp"

#include "SDL.h"

#include "Color.hpp"
#include "Chrono.hpp"

#include <algorithm>

constexpr size_t MAX_FPS = 240;
constexpr size_t MIN_FPS = 15;

static int TEST = 0;

Game::Game ()
    : m_targetFrameRate(60)
    , m_fixedUpdateTimeStep(1000/m_targetFrameRate)
    , m_pRenderer(0)
{
}

Game::~Game ()
{
    // Destroy relevant singletons/managers
}


int Game::Run ()
{
    //// Create some test objects ////

    //// Game loop ////
    
    int rc = GameErrorCode::OK;

    // Used in computing time-step
    size_t previous = Chrono::GetTicks();
    size_t current = 0;
    size_t elapsed = 0;
    size_t lag = 0;

    // Spin away!
    while (true)
    {
        // Calculate elapsed time-step
        // (Courtesy of Game Programming Patterns by Robert Nystrom)
        current = Chrono::GetTicks();
        elapsed = current - previous;
        lag += elapsed;
        previous = current;
        // console("elapsed = {}, lag = {}",  elapsed, lag);
    
        // Process all events in the SDL event queue; this is also the point at which the game loop can be exited
        if (ProcessEvents()) break;

        // Update all systems using a series of fixed time-steps
        while (lag >= m_fixedUpdateTimeStep)
        {
            lag -= m_fixedUpdateTimeStep;
        }

        // Render the game using the normalized lag
        DrawWorld(float(lag)/float(m_fixedUpdateTimeStep));

        // Consider sleeping a bit after a cycle to save power/energy on the host platform
        // TODO: Make this a configurable setting
        SDL_Delay(1);
    }

    return rc;
}

void Game::SetTargetFrameRate (size_t fps)
{
    if (fps > MAX_FPS)
    {
        fps = MAX_FPS;
    }
    else if (fps < MIN_FPS)
    {
        fps = MIN_FPS;
    }

    m_targetFrameRate = fps;
    m_fixedUpdateTimeStep = m_targetFrameRate/1000;
}

bool Game::ProcessEvents ()
{
    static SDL_Event event; // watch out for thread-safety issues with static storage - currently should be fine since this function is only ever called on the main thread

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
        {
            return true;
        }		

        switch (event.type)
        {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
                // TODO: Handle input
                break;
        }
    }

    return false; // in the normal case, we are NOT exiting the game loop
}

void Game::DrawWorld (float dt)
{
    // TODO: Use the normalized lag dt to produce a more accurate render

    // m_pRenderer->FillScreenBackground(Color::Mix(float(TEST++ & 0xfF)/float(0xFF), 1.f, 1.f));
    m_pRenderer->FillScreenBackground(Color::Orange);

    // Final step - send the frame buffer to the video device
    // TODO: should reword comment
    m_pRenderer->RenderFrame();
}