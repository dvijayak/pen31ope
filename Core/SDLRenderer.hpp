#ifndef SDLRenderer_hpp
#define SDLRenderer_hpp

#include "IRenderer.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "SDL.h"

#include "Color.hpp"
#include "Vector.hpp"
#include "ILineRasterizer.hpp"
#include "ITriangleRasterizer.hpp"

class SDLRenderer : virtual public IRenderer
{
public:
    SDLRenderer ();
    virtual ~SDLRenderer ();

    void Initialize (std::string windowTitle, uint width, uint height);

    /// IRenderer - Basic
    void SetPixel (uint index, ColorRGB color=Color::Black) override;
    void SetPixel (uint x, uint y, ColorRGB color=Color::Black) override;
    void FillScreenBackground (ColorRGB color=Color::Black) override;
    void RenderFrame () override;

    /// IRenderer - Drawing
    void DrawLine (Vector3 const& from, Vector3 const& to, ColorRGB color) override;
    void DrawTriangle (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2, ColorRGB color) override;

private:
    uint m_WIDTH;
    uint m_HEIGHT;

    // TODO: Convert all to unique ptr
    SDL_Window* m_pWindow = 0;

    // We possess ownership of screen pixels, screen texture and screen renderer
    std::vector<ColorRGB> m_pixels;
    SDL_Renderer* m_pRenderer = 0;
    SDL_Texture* m_pTexture = 0;

    std::unique_ptr<ILineRasterizer> m_pLineRasterizer;
    std::unique_ptr<ITriangleRasterizer> m_pTriangleRasterizer;
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

inline void SDLRenderer::DrawLine (Vector3 const& from, Vector3 const& to, ColorRGB color)
{
    m_pLineRasterizer->DrawLine(from, to, color);
}

inline void SDLRenderer::DrawTriangle (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2, ColorRGB color)
{
    m_pTriangleRasterizer->DrawTriangle(v0, v1, v2, color);
}


#endif