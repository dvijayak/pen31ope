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

#include "Constants.hpp"

#include "Color.hpp"
#include "Chrono.hpp"
#include "Matrix.hpp"

#include "Mesh.hpp"
#include "Triangle.hpp"

constexpr uint MAX_FPS = 240;
constexpr uint MIN_FPS = 15;

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

    m_camera.Aspect(m_screenWidth/m_screenHeight);
    UpdateViewportMatrix();
    RecreateZBuffer();
}

void Game::SetScreenHeight (float const height)
{
    m_screenHeight = height;

    m_camera.Aspect(m_screenWidth/m_screenHeight);
    UpdateViewportMatrix();
    RecreateZBuffer();
}

void Game::RecreateZBuffer()
{
    // TODO: I don't like the Z-Buffer data structure being defined in this class...
    m_zBuffer = std::vector<float>(m_screenWidth * m_screenHeight); 
    ResetZBuffer();
}

void Game::ResetZBuffer ()
{
    // TODO: Investigate whether memset actually is more efficient than std::fill or not
    // memset(m_zBuffer.data(), 0, sizeof(float) * m_zBuffer.size());
    std::fill(m_zBuffer.begin(), m_zBuffer.end(), std::numeric_limits<float>::max()); // don't use `min()`, as it doesn't work as expected for floating-point type; cf. https://en.cppreference.com/w/cpp/types/numeric_limits/lowest);
}

void Game::DrawReferenceCube (Vector3 const& center, float const s)
{
    Matrix4 const& viewMatrix = m_camera.ViewMatrix();
    Matrix4 const& projectionMatrix = m_camera.ProjectionMatrix();
    Matrix4 const& viewportMatrix = m_viewportMatrix;

    // Draw reference cube
    std::array<Vector3, 9> pointsV;
    std::array<Vector4, 9> pointsNDC;
    std::array<Vector4, 9> pointsClip;
    std::array<Vector3, 9> points{
        center + Vector3(s, s, s),
        center + Vector3(s, s, -s),
        center + Vector3(-s, s, -s),
        center + Vector3(-s, s, s),
        center + Vector3(-s, -s, s),
        center + Vector3(s, -s, s),
        center + Vector3(s, -s, -s),
        center + Vector3(-s, -s, -s),
        center
    };
    for (int i = 0; i < 9; ++i)
    {
        pointsV[i] = viewMatrix * points[i];
        pointsNDC[i] = projectionMatrix * HomoVector(pointsV[i]);
        // Perspective divide    
        points[i] = ProjectToHyperspace(pointsNDC[i]);
        points[i] = viewportMatrix * points[i];
    }

    // Render
    ColorRGB c = Color::White;
    m_pRenderer->DrawLine(points[0], points[1], c);
    m_pRenderer->DrawLine(points[1], points[2], c);
    m_pRenderer->DrawLine(points[2], points[3], c);
    m_pRenderer->DrawLine(points[3], points[0], c);
    m_pRenderer->DrawLine(points[4], points[5], c);
    m_pRenderer->DrawLine(points[5], points[6], c);
    m_pRenderer->DrawLine(points[6], points[7], c);
    m_pRenderer->DrawLine(points[7], points[4], c);
    m_pRenderer->DrawLine(points[3], points[4], c);
    m_pRenderer->DrawLine(points[0], points[5], c);
    m_pRenderer->DrawLine(points[1], points[6], c);
    m_pRenderer->DrawLine(points[2], points[7], c);
    m_pRenderer->SetPixel(points[8].x, points[8].y, c);
}

int Game::Run ()
{    
    //// Create some test objects ////

    // Setup camera
    m_camera.Fov(Constants::Deg2Rad(90));
    m_camera.Aspect(m_screenWidth/m_screenHeight);
    m_camera.Near(0.1f);
    m_camera.Far(100.f);
    m_camera.LookAt(Vector3(0, 0, 2)); // TODO: Z can't be 0 for some reason..figure that out

    // Add objects to scene
    m_objects.push_back(m_objectFactory.MakeTexturedObject("models/african_head.obj", "models/african_head_diffuse.tga"));
    // m_objects.push_back(m_objectFactory.MakeTexturedObject("models/diablo_pose.obj", "models/diablo_pose_diffuse.tga"));    
    // m_objects.push_back(m_objectFactory.MakeTexturedObject("models/cube2.obj"));
    // m_objects[0]->Translate(Vector3(-0.5f, 0, 0));
    // m_objects[0]->Rotate(0, Constants::Deg2Rad(0), 0);
    // m_objects[1]->Translate(Vector3(0.5f, 0, 0));

    // Setup lights
    m_lights.push_back(Normalized(Vector3::Backward));
    // m_lights.push_back(Normalized(Vector3(0, -2, -2)));
    // m_lights.push_back(Normalized(Vector3(20, 0, -3)));
    // m_lights.push_back(Normalized(Vector3::Forward));

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
                switch (event.key.keysym.sym)
                {
                    case SDLK_w:
                    case SDLK_a:
                    case SDLK_s:
                    case SDLK_d:
                    {
                        float factor = 0.1f;
                        float horzMovement = factor * (event.key.keysym.sym == SDLK_d ? 1 : (event.key.keysym.sym == SDLK_a ? -1 : 0));
                        float vertMovement = factor * (event.key.keysym.sym == SDLK_w ? 1 : (event.key.keysym.sym == SDLK_s ? -1 : 0));                        
                        m_lights[0] = Normalized(m_lights[0] + Vector3(horzMovement, vertMovement, 0));
                    }
                }

                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
                {
                    float movement = 0.1f;
                    movement *= event.key.keysym.sym == SDLK_LEFT ? 1 : (event.key.keysym.sym == SDLK_RIGHT ? -1 : 0);
                    // m_camera.Move(Vector3::Left * movement);
                    m_objects[0]->Rotate(0, -movement, 0);
                }

                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                {
                    float movement = 0.1f;
                    movement *= event.key.keysym.sym == SDLK_UP ? 1 : (event.key.keysym.sym == SDLK_DOWN ? -1 : 0);                    
                    // m_objects[0]->Translate(Vector3::Up * movement);
                    m_camera.Translate(Vector3::Up * movement);
                }

                if (event.key.keysym.sym == SDLK_COMMA || event.key.keysym.sym == SDLK_PERIOD)
                {
                    float delta = 2.f;
                    delta *= event.key.keysym.sym == SDLK_COMMA ? 1 : (event.key.keysym.sym == SDLK_PERIOD ? -1 : 0);
                    float current = m_camera.Fov();
                    float currentDegrees = Constants::Rad2Deg(current);
                    float nextDegrees = currentDegrees + delta;                        
                    float next = Constants::Deg2Rad(nextDegrees);
                    m_camera.Fov(next);
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
    // x, y -> Convert from [-1, 1] to [0, 1], then scale by screen dimensions    
    // z -> leave as-is
    float v_x = 0.5f * m_screenWidth, v_y = 0.5f * m_screenHeight;
    m_viewportMatrix = Matrix4(Matrix4::elements_array_type{
       v_x,   0, 0, v_x, 
         0, v_y, 0, v_y, 
         0,   0, 1,   0,
         0,   0, 0,   1, 
    });
}

void Game::DrawWorld (float dt)
{
    ResetZBuffer();    

    Matrix4 const& projectionViewMatrix = m_camera.ProjectionViewMatrix();
    Matrix4 const& viewportMatrix = m_viewportMatrix;
    
    for (auto&& obj : m_objects)
    {
        if (!obj) continue;

        Matrix4 const modelMatrixInverseTranspose = ~obj->ModelMatrixInverse();

        for (auto const& face : obj->Mesh()->GetFaces())
        {
            // Transform surface normals and compute intensity     
            Vector3 surfaceNormal = TransformDirection(modelMatrixInverseTranspose, face.Normal()); // assumes transformation results in unit vector
            float intensity = std::max(0.f, -Dot(m_lights[0], surfaceNormal));

            // Back-face culling                        
            if (Dot(m_camera.LookAtDirection(), surfaceNormal) >= 0) continue;
            
            // Transform from model space all the way to NDC clip space
            Matrix4 projectionViewModelMatrix = projectionViewMatrix * obj->ModelMatrix();
            Vector4 v0_homo = projectionViewModelMatrix * HomoVector(face[0].xyz());
            Vector4 v1_homo = projectionViewModelMatrix * HomoVector(face[1].xyz());
            Vector4 v2_homo = projectionViewModelMatrix * HomoVector(face[2].xyz());

            // Perspective divide
            Vector3 v0_ndc = ProjectToHyperspace(v0_homo);
            Vector3 v1_ndc = ProjectToHyperspace(v1_homo);
            Vector3 v2_ndc = ProjectToHyperspace(v2_homo);

            // Transform to screen space while maintaining z-coordinate for depth buffer
            Vector3 v0 = viewportMatrix * v0_ndc;
            Vector3 v1 = viewportMatrix * v1_ndc;
            Vector3 v2 = viewportMatrix * v2_ndc;
            
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
                    float l0 = baryCoords.x, l1 = baryCoords.y, l2 = baryCoords.z;
                    if (l0 >= 0 && l1 >= 0 && l2 >= 0)
                    {
                        // Interpolate UV
                        auto const& uv0 = face[0].uv();
                        auto const& uv1 = face[1].uv();
                        auto const& uv2 = face[2].uv();
                        float u_interpolated = l0 * uv0.x + l1 * uv1.x + l2 * uv2.x;
                        float v_interpolated = l0 * uv0.y + l1 * uv1.y + l2 * uv2.y;

                        // Get color from diffuse map
                        ColorRGB diffuseColor = obj->Material() && obj->Material()->DiffuseMap() ? obj->Material()->DiffuseMap()->Map(u_interpolated, v_interpolated) : face.DebugColor();

                        // Gouraud shading: interpolate normal and compute lighting intensity
                        // Assumes transformation results in unit vector
                        float intensity0 = Dot(m_lights[0], TransformDirection(modelMatrixInverseTranspose, face[0].normal()));
                        float intensity1 = Dot(m_lights[0], TransformDirection(modelMatrixInverseTranspose, face[1].normal()));
                        float intensity2 = Dot(m_lights[0], TransformDirection(modelMatrixInverseTranspose, face[2].normal()));
                        float pixelIntensity = std::max(0.f, -(l0 * intensity0 + l1 * intensity1 + l2 * intensity2));

                        // Apply lighting intensity modifier
                        ColorRGB intensifiedColor = Color::Intensify(diffuseColor, pixelIntensity); // gouraud shading
                        // ColorRGB intensifiedColor = Color::Intensify(Color::White, pixelIntensity); // gouraud shading, one color
                        // ColorRGB intensifiedColor = Color::Intensify(diffuseColor, intensity); // flat shading
                        // ColorRGB intensifiedColor = Color::Intensify(Color::White, intensity); // flat shading, one color
                        // ColorRGB intensifiedColor = diffuseColor; // no shading

                        // Handle z-buffer
                        if (m_zBuffer.empty())
                        {
                            m_pRenderer->SetPixel(x, y, intensifiedColor);
                        }
                        else
                        {
                            // Interpolate z-buffer
                            float z = l0 * v0.z + l1 * v1.z + l2 * v2.z;

                            // TODO: This "clipping" has no performance benefits at this phase; it should be done in clip space
                            if (z < -1 || z > 1) continue;

                            // Depth test: vertices closest to the near-plane pass, with -1 = near-plane, 1 = far-plane
                            uint index = y * m_screenWidth + x;
                            if (z <= m_zBuffer[index])
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

    // DrawReferenceCube();

    m_pRenderer->RenderFrame();
}