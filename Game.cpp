#include "Game.hpp"

#include "global.hpp"

#include <cmath>
#include <iostream>

#include "SDL.h"

#include "Color.hpp"
#include "Chrono.hpp"

#include "Mesh.hpp"

constexpr uint MAX_FPS = 240;
constexpr uint MIN_FPS = 15;

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

    m_objects.push_back(std::move(Mesh::MakeFromOBJ("models/african_head.obj")));

    //// Game loop ////
    
    int rc = GameErrorCode::OK;

    // Used in computing time-step
    uint previous = Chrono::GetTicks();
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

static uint COUNT = 0;

void Game::DrawWorld (float dt)
{
    // TODO: Use the normalized lag dt to produce a more accurate render

    float const theta = (COUNT++/1000.f) * (2*M_PI);
    float const r = std::min(m_screenWidth, m_screenHeight) / 2.0f;
    float const x = r * cos(theta);
    float const y = -r * sin(theta);
    float const x_center = m_screenWidth/2.f;
    float const y_center = m_screenHeight/2.f;
    m_pRenderer->DrawLine(x_center, y_center, x_center + x, y_center + y, Color::Orange);

    for (auto&& obj : m_objects)
    {
        if (!obj) continue;
        for (auto face : obj->GetFaces())
        {
            for (int i = 0; i < 3; i++)
            {
                // Convert from [-1, 1] to [0, 1], then scale by screen dimensions
                // Y coordinates are flipped in screen spacw
                float x1 = (face[i].x() + 1.f) * 0.5 * m_screenWidth;
                float y1 = (face[i].y() * -1.f + 1.f) * 0.5 * m_screenHeight;
                float x2 = (face[(i + 1) % 3].x() + 1.f) * 0.5 * m_screenWidth;
                float y2 = (face[(i + 1) % 3].y() * -1.f + 1.f) * 0.5 * m_screenHeight;

                m_pRenderer->DrawLine(x1, y1, x2, y2, Color::White);
            }
        }
    }

    m_pRenderer->RenderFrame();
}