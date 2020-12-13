#include "SDLRenderer.hpp"

#include <cmath>
#include <cstring>

#include "SDL_image.h"

#include "Logger.hpp"
#include "LerpLineRasterizer.hpp"
#include "BresenhamsLineRasterizer.hpp"
#include "LerpTriangleRasterizer.hpp"
// #include "BarycentricTriangleRasterizer.hpp"

SDLRenderer::SDLRenderer ()
{}

static void DumpSDLVersion ()
{
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    std::cout << "We compiled against SDL version " << compiled.major << ", " << compiled.minor << ", " << compiled.patch << std::endl;
    std::cout << "We are linking against SDL version " << linked.major << ", " << linked.minor << ", " << linked.patch << std::endl;
}

void SDLRenderer::Initialize (std::string windowTitle, uint width, uint height, int windowFlags)
{
    // TODO: Doesn't show the version, not sure why
    // DumpSDLVersion();

    trclog("Initializing SDL...");

    SDL_Init(SDL_INIT_VIDEO);

    // Window
    m_pWindow = SDL_CreateWindow(windowTitle.c_str(), 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        windowFlags
        // SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN
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
    m_rowBuffer = std::vector<ColorRGB>(m_WIDTH);

    // Initialize rasterizers
    // m_pLineRasterizer = std::make_unique<LerpLineRasterizer>(this);
    m_pLineRasterizer = std::make_unique<BresenhamsLineRasterizer>(this);
    m_pTriangleRasterizer = std::make_unique<LerpTriangleRasterizer>(this, m_pLineRasterizer.get());
    // {
    //     std::unique_ptr<BarycentricTriangleRasterizer> btr(new BarycentricTriangleRasterizer(this));
    //     btr->UpdateScreenResolution(m_WIDTH, m_HEIGHT);
    //     m_pTriangleRasterizer = std::move(btr);
    // }

    trclog("\tRenderer initialized.");

    // Init JPG and PNG support for SDL_Image
    int sdlImgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    int sdlImgInitResult = IMG_Init(sdlImgFlags);
    if ((sdlImgInitResult & sdlImgFlags) != sdlImgFlags)
    {
        trclog("\tFailed to initialize PNG and JPG image support. IMG_Init error: " << IMG_GetError());
        // TODO: Set a flag so that renderer does not do things that require PNG/JPG support
    }

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

    IMG_Quit();
    SDL_Quit();

    trclog("SDL shutdown complete.");
}

void SDLRenderer::RenderFrame ()
{
    FlipFrameVertically();

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

void SDLRenderer::FlipFrameVertically ()
{
    ColorRGB * pixels = m_pixels.data();
    
    // Algorithm: Just like reversing a string, except each character is actually a row of pixels

    // Start with two pointers, one to the first row, the other to the last row
    uint row1 = 0, row2 = m_HEIGHT-1;
    
    // Temp buffer used for swapping
    ColorRGB * tmp = m_rowBuffer.data();

    // Until the two pointers are pointing to the same row or have passed each other, swap the two rows being pointed to
    for (; row1 < row2; ++row1, --row2)
    {
        ColorRGB * a = pixels + row1*m_WIDTH;
        ColorRGB * b = pixels + row2*m_WIDTH;

        // Swap rows
        memcpy(tmp, a, m_WIDTH * sizeof(ColorRGB));
        memcpy(a,   b, m_WIDTH * sizeof(ColorRGB));
        memcpy(b, tmp, m_WIDTH * sizeof(ColorRGB));
    }    
}
