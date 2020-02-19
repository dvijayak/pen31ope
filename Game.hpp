#ifndef Game_HPP
#define Game_HPP

#include <memory>
#include <utility> // for std::pair
#include <vector>

#include "IRenderer.hpp"
#include "Vector3.hpp"

class Mesh;

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

    void SetRenderer (IRenderer* pRM) { m_pRenderer = pRM; }

    void SetScreenWidth (float width) { m_screenWidth = width; }
    void SetScreenHeight (float height) { m_screenHeight = height; }

    /**
     * Performed once normally at the beginning of each frame
     */
    bool ProcessEvents ();

    /**
     * Performed once normally at the end of each frame
     */
    void DrawWorld (float dt); // dt => normalized lag, i.e. how far into the next frame update cycle the game loop is currently in

private:
    /**
     * Convert from [-1, 1] to [0, 1], then scale by screen dimensions
     */
    Vector3 NDCToScreenPixels (Vector3 const& v) const;

    size_t m_targetFrameRate; // FPS
    size_t m_fixedUpdateTimeStep; // milliseconds, normally synced to target frame rate

    IRenderer* m_pRenderer; // TODO: Change to unique_ptr?

    // TODO: these should update as the window is resized. I should probably implement
    // the Observer pattern in due time...
    float m_screenWidth;
    float m_screenHeight;

    std::vector<std::unique_ptr<Mesh>> m_objects;
};

#endif