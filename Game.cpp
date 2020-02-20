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
    // m_objects.push_back(std::move(Mesh::MakeFromOBJ("models/diablo_pose.obj")));

    m_lights.push_back(Vector3(0, 0, -1.f).Normalized());

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

Vector3 Game::NDCToScreenPixels (Vector3 const& v) const
{
    return Vector3(
        (v.x() + 1.f) * 0.5f * m_screenWidth,
        (v.y() * -1.f + 1.f) * 0.5f * m_screenHeight // y-coordinates are flipped in screen space
        );
}

static uint COUNT = 0;

void Game::DrawWorld (float dt)
{
    // TODO: Use the normalized lag dt to produce a more accurate render

    ColorRGB color = Color::White;

    for (auto&& obj : m_objects)
    {
        if (!obj) continue;
        for (auto const& face : obj->GetFaces())
        {            
            float intensity = -m_lights[0].Dot(face.Normal());

            // Back-face culling
            if (intensity <= 0) continue;
            
            ColorRGB intensifiedColor = Color::Intensify(color, intensity);

            Vector3 v0 = NDCToScreenPixels(Vector3(face[0].x(), face[0].y()));
            Vector3 v1 = NDCToScreenPixels(Vector3(face[1].x(), face[1].y()));
            Vector3 v2 = NDCToScreenPixels(Vector3(face[2].x(), face[2].y()));
            m_pRenderer->DrawTriangle(v0.x(), v0.y(), v1.x(), v1.y(), v2.x(), v2.y(), intensifiedColor);
        }
    }

    m_pRenderer->RenderFrame();
}