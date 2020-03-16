#include "Game.hpp"

#include "global.hpp"

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

#ifdef WIN32
#define NOMINMAX
#include "Windows.h"
#include <sstream>
#endif

#include "SDL.h"
#include "SDL_image.h"

#include "Color.hpp"
#include "Chrono.hpp"
#include "Matrix.hpp"

#include "Mesh.hpp"
#include "Triangle.hpp"

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

void Game::SetScreenWidth (float const width)
{
    m_screenWidth = width;
    UpdateViewportMatrix();
}

void Game::SetScreenHeight (float const height)
{
    m_screenHeight = height;
    UpdateViewportMatrix();
}

int Game::Run ()
{
    // TODO: I don't like the Z-Buffer data structure being defined in this class...
    m_zBuffer = std::vector<float>(m_screenWidth * m_screenHeight, std::numeric_limits<float>::lowest()); // don't use `min()`, as it doesn't work as expected for floating-point type; cf. https://en.cppreference.com/w/cpp/types/numeric_limits/lowest);

    //// Create some test objects ////

    m_camera.LookAt(Vector3(1, 1, 3));

    m_objects.push_back(m_objectFactory.MakeTexturedObject("models/african_head.obj", "models/african_head_diffuse.tga"));
    // m_objects.push_back(m_objectFactory.MakeTexturedObject("models/diablo_pose.obj", "models/diablo_pose_diffuse.tga"));

    m_lights.push_back(Normalized(Vector3::Backward));

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
        #if defined(WIN32)
        std::stringstream ss;
        ss << "elapsed = " << elapsed << ", lag = " << lag << std::endl;
        OutputDebugStringA(ss.str().c_str());
        #else
        // std::cout << "elapsed = " << elapsed << ", lag = " << lag << std::endl;
        #endif
    
        // Process all events in the SDL event queue; this is also the point at which the game loop can be exited
        if (ProcessEvents()) break;

        // Update all systems using a series of fixed time-steps
        while (lag >= m_fixedUpdateTimeStep)
        {
            lag -= m_fixedUpdateTimeStep;
        }

        // Render the game using the normalized lag
        DrawWorld(float(lag)/float(m_fixedUpdateTimeStep));

        #ifdef NDEBUG
        // Consider sleeping a bit after a cycle to save power/energy on the host platform
        SDL_Delay(1);
        #endif
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
                // TODO: Just for testing
                {
                    float movement = 0.1f;
                    movement *= event.key.keysym.sym == SDLK_LEFT ? 1 : (event.key.keysym.sym == SDLK_RIGHT ? -1 : 0);
                    m_lights[0] = Normalized(m_lights[0] + (Vector3::Left * movement));
                    // m_camera.Move(Vector3::Left * movement);
                }
                break;
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

void Game::UpdateViewportMatrix ()
{
    // Convert from [-1, 1] to [0, 1], then scale by screen dimensions    
    float v_x = 0.5f * m_screenWidth, v_y = 0.5f * m_screenHeight;
    m_viewportMatrix = Matrix4(Matrix4::elements_array_type{
       v_x,   0, 0, v_x, 
         0, v_y, 0, v_y, 
         0,   0, 1,   0,  // TODO: This is incomplete....need to transform to NDC CUBE, NOT rectangle...see https://github.com/ssloy/tinyrenderer/wiki/Lesson-5-Moving-the-camera#viewport
         0,   0, 0,   1, 
    });
}

void Game::DrawWorld (float dt)
{
    ColorRGB color = Color::White;
 
    Matrix4 const& screenProjectionViewModelMatrix = m_viewportMatrix * m_camera.ProjectionViewMatrix();
    
    for (auto&& obj : m_objects)
    {
        if (!obj) continue;

        for (auto const& face : obj->Mesh()->GetFaces())
        {
            float intensity = Dot(m_lights[0], face.Normal());

            // Back-face culling                        
            if (Dot(m_camera.LookAtDirection(), face.Normal()) >= 0) continue;
            
            // Apply perspective projection, then transform to screen space, maintaining z-component
            Vector3 v0 = screenProjectionViewModelMatrix * face[0].xyz();
            Vector3 v1 = screenProjectionViewModelMatrix * face[1].xyz();
            Vector3 v2 = screenProjectionViewModelMatrix * face[2].xyz();
            
            // Compute minimum rectangle that fully contains the 3 vertices in screen space
            auto const boundingBox = TriangleUtil::MinimumBoundingBox<float>(v0, v1, v2)
                                        .Clip(Box2(Vector2(0, 0), Vector2(m_screenWidth, m_screenHeight)));
            uint x_start = boundingBox.bottomLeft.x, y_start = boundingBox.bottomLeft.y;
            uint x_end =  boundingBox.topRight.x, y_end = boundingBox.topRight.y;

            // Identify the pixels within the bounds and compute their colour
            for (uint x = x_start; x <= x_end; ++x)
            {                
                for (uint y = y_start; y <= y_end; ++y)
                {
                    Vector3 baryCoords = TriangleUtil::BarycentricCoordinates(Vector3(x, y), v0, v1, v2);
                    float u = baryCoords.x, v = baryCoords.y, w = baryCoords.z; // TODO: Rename bary coord names from
                    if (u >= 0 && v >= 0 && w >= 0)
                    {
                        // Interpolate UV
                        auto const& uv0 = face[0].uv();
                        auto const& uv1 = face[1].uv();
                        auto const& uv2 = face[2].uv();
                        float u_interpolated = u * uv0.x + v * uv1.x + w * uv2.x;
                        float v_interpolated = u * (1.f - uv0.y) + v * (1.f - uv1.y) + w * (1.f - uv2.y); // subtract y from 1 since y-axis is flipped in screen space

                        // Get color from diffuse map
                        ColorRGB diffuseColor = obj->Material() && obj->Material()->DiffuseMap() ? obj->Material()->DiffuseMap()->Map(u_interpolated, v_interpolated) : Color::White;

                        // Gouraud shading: interpolate normal and compute lighting intensity
                        float intensity0 = Dot(m_lights[0], face[0].normal());
                        float intensity1 = Dot(m_lights[0], face[1].normal());
                        float intensity2 = Dot(m_lights[0], face[2].normal());
                        float pixelIntensity = u * intensity0 + v * intensity1 + w * intensity2;

                        // Apply lighting intensity modifier
                        // ColorRGB intensifiedColor = Color::Intensify(diffuseColor, -pixelIntensity); // gouraud shading
                        ColorRGB intensifiedColor = Color::Intensify(Color::White, -pixelIntensity); // gouraud shading, one color
                        // ColorRGB intensifiedColor = Color::Intensify(diffuseColor, -intensity); // flat shading
                        // ColorRGB intensifiedColor = Color::Intensify(Color::White, -intensity); // flat shading, one color
                        // ColorRGB intensifiedColor = diffuseColor; // no shading

                        // Handle z-buffer
                        if (m_zBuffer.empty())
                        {
                            m_pRenderer->SetPixel(x, y, intensifiedColor);
                        }
                        else
                        {
                            // Interpolate z-buffer
                            float z = u * v0.z + v * v1.z + w * v2.z;
                            uint index = y * m_screenWidth + x;
                            if (z >= m_zBuffer[index])
                            {
                                m_zBuffer[index] = z;
                                m_pRenderer->SetPixel(x, y, intensifiedColor);
                            }
                        }   
                    }
                }
            }
        }
    }

    m_pRenderer->RenderFrame();
}
