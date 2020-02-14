#include "SDLRenderer.hpp"

#include <cmath>

#include "Logger.hpp"

SDLRenderer::SDLRenderer ()
{}

void SDLRenderer::Initialize (std::string windowTitle, size_t width, size_t height)
{
    trclog("Initializing SDL...");

    SDL_Init(SDL_INIT_VIDEO);

    // Window
    m_pWindow = SDL_CreateWindow(windowTitle.c_str(), 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE
        );

    m_WIDTH = width;
    m_HEIGHT = height;

    trclog("\tWindow initialized.");
        
    // Renderer
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Main Texture
    m_pTexture = SDL_CreateTexture(m_pRenderer, 
        SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_STATIC,
        m_WIDTH, m_HEIGHT
        );

    // Raw buffer
    m_pixels = std::vector<ColorRGB>(m_WIDTH * m_HEIGHT);

    trclog("\tRenderer initialized.");

    trclog("SDL initialization complete.");
}

SDLRenderer::~SDLRenderer ()
{
	trclog("Shutting down SDL...");

    SDL_DestroyWindow(m_pWindow);

    trclog("\tWindow destroyed.");  

    SDL_DestroyTexture(m_pTexture);
    SDL_DestroyRenderer(m_pRenderer);

    trclog("\tRenderer destroyed.");

    SDL_Quit();

    trclog("SDL shutdown complete.");
}

void SDLRenderer::RenderFrame () const
{
    SDL_UpdateTexture(m_pTexture, 0, m_pixels.data(), m_WIDTH * sizeof(Uint32));

    SDL_RenderClear(m_pRenderer);
    SDL_RenderCopy(m_pRenderer, m_pTexture, 0, 0);
    SDL_RenderPresent(m_pRenderer);
}

void SDLRenderer::FillScreenBackground (ColorRGB color)
{
    for (size_t w = 0; w < m_WIDTH; ++w)
    {
        for (size_t h = 0; h < m_HEIGHT; ++h)
        {
            SetPixel(w, h, color);
        }
    }
}

// void SDLRenderer::DrawPoint (PointF const& p, Uint32 color)
// {
// 	SetPixel(p.x * m_WIDTH, p.y * m_HEIGHT, color);
// }

// void SDLRenderer::DrawLine (float x_s, float y_s, float x_e, float y_e, Uint32 color)
// {
// 	// Transform the normalized device coordinates to actual device positions
// 	x_s *= m_WIDTH;
// 	y_s *= m_HEIGHT;
// 	x_e *= m_WIDTH;
// 	y_e *= m_HEIGHT;

//  	// console("Start = ({},{})\tEnd = ({},{})", x_s, y_s, x_e, y_e);

//  	// Cut short points that extend beyond the display's dimensions
//  	if (x_s < 0)
//  		x_s = 0; 
//  	else if (x_s >= m_WIDTH)
//  		x_s = m_WIDTH-1;
//  	if (x_e < 0)
//  		x_e = 0;
//  	else if (x_e >= m_WIDTH)
//  		x_e = m_WIDTH-1;
//  	if (y_s < 0)
//  		y_s = 0; 
//  	else if (y_s >= m_HEIGHT)
//  		y_s = m_HEIGHT-1;
//  	if (y_e < 0)
//  		y_e = 0;
//  	else if (y_e >= m_HEIGHT)
//  		y_e = m_HEIGHT-1;

//  	// console("(CUT) Start = ({},{})\tEnd = ({},{})", x_s, y_s, x_e, y_e);

// 	// If the start and end are the same, there's nothing else to be done
// 	if (x_s == x_e && y_s == y_e)
// 	{
// 		return;
// 	}

// 	// Approximate the line segment and render each discrete point on the line
// 	float dx = x_e - x_s;
// 	float dy = y_e - y_s;

// 	// Corner case 1: dy = 0, so dx/dy = n.d
// 	// Simply fill up every single pixel along the x-axis
// 	if (dy == 0.0)
// 	{
// 		int y = int(y_s);
// 		if (dx < 0.0)
// 		{
// 			for (int x = int(x_s); x > x_e; --x)
// 			{
// 				SetPixel(x, y, color);
// 			}
// 		}
// 		else
// 		{
// 			for (int x = int(x_s); x < x_e; ++x)
// 			{
// 				SetPixel(x, y, color);
// 			}
// 		}
// 	}
// 	// Corner case 2: dx = 0, so dy/dx = n.d
// 	// Simply fill up every single pixel along the y-axis
// 	else if (dx == 0.0)
// 	{
// 		int x = int(x_s);
// 		if (dy < 0.0)
// 		{
// 			for (int y = int(y_s); y > y_e; --y)
// 			{
// 				SetPixel(x, y, color);
// 			}
// 		}
// 		else
// 		{
// 			for (int y = int(y_s); y < y_e; ++y)
// 			{
// 				SetPixel(x, y, color);
// 			}
// 		}
// 	}
// 	// Corner case 3: dx = dy, so dy/dx = 1
// 	// Rarely occurs, but why not handle it?
// 	else if (std::abs(dx) == std::abs(dy))
// 	{
// 		int x = int(std::round(x_s));
// 		int y = int(std::round(y_s));

// 		if (dy > 0.0)
// 		{
// 			if (dx > 0.0)
// 			{
// 				for (; x < x_e && y < y_e; ++x, ++y)
// 				{
// 					SetPixel(x, y, color);
// 				}
// 			}
// 			else // dx < 0
// 			{
// 				for (; x > x_e && y < y_e; --x, ++y)
// 				{
// 					SetPixel(x, y, color);
// 				}
// 			}
// 		}
// 		else // dy < 0
// 		{
// 			if (dx > 0.0)
// 			{
// 				for (; x < x_e && y > y_e; ++x, --y)
// 				{
// 					SetPixel(x, y, color);
// 				}
// 			}
// 			else // dx < 0
// 			{
// 				for (; x > x_e && y > y_e; --x, --y)
// 				{
// 					SetPixel(x, y, color);
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		// Now the real (no pun intended) work begins: deal with the octants

// 		float m = dy/dx;

// 		if (m < -1.0) // |dy| > |dx|, so approximate x component
// 		{
// 			m = 1/m;

// 			if (dy < 0.0) // Octant 0
// 			{
// 				std::swap(x_s, x_e);
// 				std::swap(y_s, y_e);
// 			}
// 			// else, Octant 4

// 			float x = x_s;
// 			for (int y = int(y_s); y < y_e; ++y, x += m)
// 			{
// 				SetPixel(int(std::round(x)), y, color);
// 			}
// 		}
// 		else if (m > 1.0) // |dy| > |dx|, so approximate x component
// 		{
// 			m = 1/m;

// 			if (dy < 0.0) // Octant 7
// 			{
// 				std::swap(x_s, x_e);
// 				std::swap(y_s, y_e);
// 			}
// 			// else, Octant 3
            
// 			float x = x_s;
// 			for (int y = int(y_s); y < y_e; ++y, x += m)
// 			{
// 				SetPixel(int(std::round(x)), y, color);
// 			}
// 		}
// 		else if (m > -1.0 && m < 0.0) // |dy| < |dx|, so approximate y component
// 		{
// 			if (dy > 0.0) // Octant 5
// 			{
// 				std::swap(x_s, x_e);
// 				std::swap(y_s, y_e);
// 			}
// 			// else, Octant 1
            
// 			float y = y_s;
// 			for (int x = int(x_s); x < x_e; ++x, y += m)
// 			{
// 				SetPixel(x, int(std::round(y)), color);
// 			}
// 		}
// 		else if (m > 0.0 && m < 1.0) // |dy| < |dx|, so approximate y component
// 		{
// 			if (dy < 0.0) // Octant 6
// 			{
// 				std::swap(x_s, x_e);
// 				std::swap(y_s, y_e);
// 			}
// 			// else, Octant 2
            
// 			float y = y_s;
// 			for (int x = int(x_s); x < x_e; ++x, y += m)
// 			{
// 				SetPixel(x, int(std::round(y)), color);
// 			}
// 		}
// 		else
// 		{
// 			errlog("Should never reach here.");
// 			assert(false);
// 		}
// 	}

// }

// void SDLRenderer::DrawLine (const LineSegmentF& seg, Uint32 color)
// {
// 	// Forget 3D for now

// 	DrawLine(seg.start.x, seg.start.y, seg.end.x, seg.end.y, color);
// }

// void SDLRenderer::DrawPolygon (const PolygonF& poly, Uint32 color)
// {
// 	// Render each edge of the polygon.
// 	std::vector<LineSegmentF> edges;
// 	poly.ComputeEdges(edges);
// 	for (auto& edge : edges)
// 	{
// 		DrawLine(edge.start.x, edge.start.y, edge.end.x, edge.end.y, color);
// 	}
// }