#ifndef SDLRenderer_hpp
#define SDLRenderer_hpp

#include "IRenderer.hpp"

#include <string>
#include <vector>

#include "SDL.h"

#include "Color.hpp"

class SDLRenderer : virtual public IRenderer
{
public:
	SDLRenderer ();
	virtual ~SDLRenderer ();

    void Initialize (std::string windowTitle, size_t width, size_t height);

    /// Helpers
	void FillScreenBackground (ColorRGB color=Color::Black);

    // TODO: Redraw
	/// All draw functions expect normalized device coordinates of range [0, 1]
	// void DrawPoint (PointF const&, Uint32 color=Color::White);
	// void DrawLine (float, float, float, float, Uint32 color=Color::White); 
	// void DrawLine (const LineSegmentF& seg, Uint32 color=Color::White);
	// void DrawPolygon (const PolygonF& poly, Uint32 color=Color::White);

    /// IRenderer
	void SetPixel (size_t index, ColorRGB color=Color::Black);
	void SetPixel (size_t x, size_t y, ColorRGB color=Color::Black);
	void RenderFrame () const;

private:
	size_t m_WIDTH;
	size_t m_HEIGHT;

    // TODO: Convert all to unique ptr
    SDL_Window* m_pWindow = 0;

	// We possess ownership of screen pixels, screen texture and screen renderer
	std::vector<ColorRGB> m_pixels; // TODO: Change this to ColorRGB array?
	SDL_Renderer* m_pRenderer = 0;
	SDL_Texture* m_pTexture = 0;
};

inline void SDLRenderer::SetPixel (size_t index, ColorRGB color)
{
	m_pixels[index] = color;
}

inline void SDLRenderer::SetPixel (size_t x, size_t y, ColorRGB color)
{
	SetPixel(y*m_WIDTH + x, color);
}

#endif