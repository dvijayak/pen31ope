#ifndef SDLRenderer_hpp
#define SDLRenderer_hpp

#include "IRenderer.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "SDL.h"

#include "Color.hpp"
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
    void DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color) override;
    void DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color) override;

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

inline void SDLRenderer::DrawLine (uint x_s, uint y_s, uint x_e, uint y_e, ColorRGB color)
{
    m_pLineRasterizer->DrawLine(x_s, y_s, x_e, y_e, color);
}

inline void SDLRenderer::DrawTriangle (uint x0, uint y0, uint x1, uint y1, uint x2, uint y2, ColorRGB color)
{
    m_pTriangleRasterizer->DrawTriangle(x0, y0, x1, y1, x2, y2, color);
}


#endif