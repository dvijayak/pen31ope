#ifndef SDLRenderer_hpp
#define SDLRenderer_hpp

#include "IRenderer.hpp"

#include <string>
#include <vector>

#include "SDL.h"

#include "Color.hpp"
#include <iostream>

class SDLRenderer : virtual public IRenderer
{
public:
    SDLRenderer ();
    virtual ~SDLRenderer ();

    void Initialize (std::string windowTitle, uint width, uint height);

    /// Helpers

    // TODO: Redraw
    /// All draw functions expect normalized device coordinates of range [0, 1]
    // void DrawPoint (PointF const&, Uint32 color=Color::White);
    // void DrawLine (float, float, float, float, Uint32 color=Color::White); 
    // void DrawLine (const LineSegmentF& seg, Uint32 color=Color::White);
    // void DrawPolygon (const PolygonF& poly, Uint32 color=Color::White);

    /// IRenderer
    void SetPixel (uint index, ColorRGB color=Color::Black) override;
    void SetPixel (uint x, uint y, ColorRGB color=Color::Black) override;
    void FillScreenBackground (ColorRGB color=Color::Black) override;
    void RenderFrame () override;

private:
    uint m_WIDTH;
    uint m_HEIGHT;

    // TODO: Convert all to unique ptr
    SDL_Window* m_pWindow = 0;

    // We possess ownership of screen pixels, screen texture and screen renderer
    std::vector<ColorRGB> m_pixels;
    SDL_Renderer* m_pRenderer = 0;
    SDL_Texture* m_pTexture = 0;
};

inline void SDLRenderer::SetPixel (uint index, ColorRGB color)
{
    if (index < m_WIDTH * m_HEIGHT)
        m_pixels[index] = color;
}

inline void SDLRenderer::SetPixel (uint x, uint y, ColorRGB color)
{
    if (x < m_WIDTH && y < m_HEIGHT)
        m_pixels[y*m_WIDTH + x] = color;
}

#endif