#ifndef Game_HPP
#define Game_HPP

#include <memory>
#include <utility> // for std::pair
#include <vector>

#include "SDLRenderer.hpp"
#include "SDLTextFactory.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Object3D.hpp"
#include "Object3DFactory.hpp"
#include "Camera.hpp"

class Game
{
public:
    enum GameErrorCode
    {
        OK = 0,
        FAIL_UNKNOWN
    };

    Game ();
    ~Game ();

    /**
     * Set the FPS. Also resynchronizes the update time-step
     */
    void SetTargetFrameRate (size_t);

    /**
     *  Game loop. Expected to be called only once at the beginning of the game.
     */
    int Run ();

    void SetRenderer (SDLRenderer* pRM) { m_pRenderer = pRM; }
    void SetTextRenderer (SDLTextFactory* pTF) { m_pTF = pTF; }

    void SetScreenWidthAndHeight (float width, float height); // it is important to call this at least once before either SetScreenWidth or SetScreenHeight are called
    void SetScreenWidth (float width);
    void SetScreenHeight (float height);

    /**
     * Performed once normally at the beginning of each frame
     */
    bool ProcessEvents ();

    /**
     * Performed once normally at the end of each frame
     */
    void DrawWorld (float dt); // dt => normalized lag, i.e. how far into the next frame update cycle the game loop is currently in

private:
    void UpdateViewportMatrix (); 
    void RecreateZBuffer ();
    void ResetZBuffer ();

    void DrawReferenceCube (Vector3 const& position=Vector3(), float const s=0.25f);

    size_t m_targetFrameRate; // FPS
    size_t m_fixedUpdateTimeStep; // milliseconds, normally synced to target frame rate

    SDLRenderer* m_pRenderer; // TODO: Change to unique_ptr?
    SDLTextFactory* m_pTF; // TODO: Change to unique_ptr?

    // TODO: these should update as the window is resized. I should probably implement
    // the Observer pattern in due time...
    float m_screenWidth;
    float m_screenHeight;
    std::vector<float> m_zBuffer;

    Object3DFactory m_objectFactory;
    std::vector<Object3D> m_objects;
    std::vector<Vector3> m_lights;

    Camera m_camera;

    Matrix4 m_viewportMatrix;
};

#endif