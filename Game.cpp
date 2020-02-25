#include "Game.hpp"

#include "global.hpp"

#include <cmath>
#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

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
        std::cout << "elapsed = " << elapsed << ", lag = " << lag << std::endl;
    
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

Vector3 Game::NDCToScreenPixels (Vector3 const& v) const
{
    return Vector3(
        (v.x + 1.f) * 0.5f * m_screenWidth,
        (v.y * -1.f + 1.f) * 0.5f * m_screenHeight, // y-coordinates are flipped in screen space
        v.z
        );
}

// TODO: REMOVE once PoC has been properly implemented
class POC_TextureMapper
{
private:
    std::vector<float> m_zBuffer;
    IRenderer* m_pRenderer;
    uint m_zBufferWidth;

public:
    POC_TextureMapper(IRenderer* pRenderer, uint width, uint height)
        : m_pRenderer(pRenderer)
        , m_zBufferWidth(width)
    {
        m_zBuffer = std::vector<float>(width * height, std::numeric_limits<float>::lowest()); 
    }

    void DrawTriangle (std::array<Vector3, 3> const& vertices, std::array<Vector2, 3> const& uvs, Object3D* object, ColorRGB alternateColor, float intensity)
    {
        auto const boundingBox = TriangleUtil::MinimumBoundingBox(vertices);
   
        uint x_start = boundingBox.topLeft[0], y_start = boundingBox.topLeft[1];
        uint x_end = boundingBox.bottomRight[0], y_end = boundingBox.bottomRight[1];
        for (uint x = x_start; x <= x_end; ++x)
        {
            for (uint y = y_start; y <= y_end; ++y)
            {
                Vector3 baryCoords = TriangleUtil::BarycentricCoordinates(Vector3(x, y), vertices);

                float u = baryCoords.x, v = baryCoords.y, w = baryCoords.z;
                if (u >= 0 && v >= 0 && w >= 0)
                {
                    Vector2 interpolatedUV(
                        u * uvs[0].x + v * uvs[1].x + w * uvs[2].x,
                        u * (1.f - uvs[0].y) + v * (1.f - uvs[1].y) + w * (1.f - uvs[2].y)
                    );
                    ColorRGB diffuseColor = Color::Intensify(object->DiffuseColorFromTexture(interpolatedUV), intensity, 1.f);
                    // ColorRGB diffuseColor = object->DiffuseColorFromTexture(interpolatedUV);
                    if (m_zBuffer.empty())
                    {
                        m_pRenderer->SetPixel(x, y, diffuseColor);
                        // m_pRenderer->SetPixel(x, y, alternateColor);
                    }
                    else
                    {
                        float z = u * vertices[0].z + v * vertices[1].z + w * vertices[2].z;
                        uint index = y * m_zBufferWidth + x;
                        if (z >= m_zBuffer[index])
                        {
                            m_zBuffer[index] = z;
                            m_pRenderer->SetPixel(x, y, diffuseColor);
                            // m_pRenderer->SetPixel(x, y, alternateColor);
                        }
                    }   
                }
            }
        }
    }
};
POC_TextureMapper* s_textureMapperPOC = nullptr;

void Game::DrawWorld (float dt)
{
    // TODO: Use the normalized lag dt to produce a more accurate render
    if (s_textureMapperPOC == nullptr)
    {
        s_textureMapperPOC = new POC_TextureMapper(m_pRenderer, m_screenWidth, m_screenHeight);
    }

    ColorRGB color = Color::White;
    
    for (auto&& obj : m_objects)
    {
        if (!obj) continue;

        // // TODO :TEST
        // for (int i = 0; i < 1024; i++)
        //     for (int j = 0; j < 1024; j++)
        //         m_pRenderer->SetPixel(i, j, obj->DiffuseColorFromTexture(
        //             float(i)/1024, float(j)/1024
        //             ));

        for (auto const& face : obj->Mesh()->GetFaces())
        {
            float intensity = Dot(m_lights[0], face.Normal());

            // Back-face culling
            if (intensity >= 0) continue;
            
            ColorRGB intensifiedColor = Color::Intensify(color, -intensity);
            Vector3 v0 = NDCToScreenPixels(face[0]);
            Vector3 v1 = NDCToScreenPixels(face[1]);
            Vector3 v2 = NDCToScreenPixels(face[2]);
            // m_pRenderer->DrawTriangle(v0, v1, v2, intensifiedColor);
            s_textureMapperPOC->DrawTriangle(std::array<Vector3, 3>{v0, v1, v2}, face.UVs(), obj, intensifiedColor, -intensity);            
        }
    }

    m_pRenderer->RenderFrame();
}