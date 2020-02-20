#include "SDLRenderer.hpp"

#include <cmath>
#include <cstring>

#include "Logger.hpp"

SDLRenderer::SDLRenderer ()
{}

void SDLRenderer::Initialize (std::string windowTitle, uint width, uint height)
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
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC,
        m_WIDTH, m_HEIGHT
        );
    SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_NONE); // no alpha blending - we will implement this ourselves; this effectively ignores the value of the alpha channel in the pixel color

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

void SDLRenderer::RenderFrame ()
{
    SDL_UpdateTexture(m_pTexture, 0, m_pixels.data(), m_WIDTH * sizeof(ColorRGB));

    SDL_RenderClear(m_pRenderer);
    SDL_RenderCopy(m_pRenderer, m_pTexture, 0, 0);
    SDL_RenderPresent(m_pRenderer);

    // Reset all pixels to black
    memset(m_pixels.data(), 0, m_WIDTH * m_HEIGHT * sizeof(ColorRGB));
}

void SDLRenderer::FillScreenBackground (ColorRGB color)
{
    for (uint w = 0; w < m_WIDTH; ++w)
    {
        for (uint h = 0; h < m_HEIGHT; ++h)
        {
            SetPixel(w, h, color);
        }
    }
}
