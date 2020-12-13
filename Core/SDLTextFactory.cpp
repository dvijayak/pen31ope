#include "SDLTextFactory.hpp"

#include <cassert>

#include "Logger.hpp"

#define DEFAULT_FONT_SIZE = 16;

struct TTFFont
{
   TTFFont(TTF_Font* p) : m_resource(p)
   {
      /* do nothing */
   }

   ~TTFFont()
   {
      TTF_CloseFont(m_resource);
   }

   TTF_Font* get() const { return m_resource; }

private:
   TTF_Font* m_resource;
};

SDLTextFactory::SDLTextFactory(SDL_Renderer* p)
   : m_pRenderer(p)
{
   assert(p != nullptr);

   if (TTF_Init() == -1)
   {
      trclog("\tFailed to initialize text rendering support. TTF_Init error: " << TTF_GetError());
   }
   else
   {
      ready = true;
   }
}

SDLTextFactory::~SDLTextFactory()
{
   ready = false;
   TTF_Quit();
}

void SDLTextFactory::Initialize()
{
   // Load up all font assets
   // TODO: Reading and loading the font file every time when we want to use it is bad. We should optimize by loading the file blob into memory, then using SDL_RWops to create a stream from memory that can then be consumed by TTF_OpenFontRW
   m_baseFonts.insert(std::make_pair(FontType::MONOSPACE, "fonts/monospace/monofont/MONOFONT.ttf"));
}


std::shared_ptr<SDLTexture> SDLTextFactory::DrawText(std::string const& text, int fontSize, FontStyle style)
{
   if (!ready)
      return nullptr;

   // Prepare TTF_Font style flags
   int styleFlags = TTF_STYLE_NORMAL;
   if (style.bold)
      styleFlags |= TTF_STYLE_BOLD;
   if (style.italic)
      styleFlags |= TTF_STYLE_ITALIC;
   if (style.underline)
      styleFlags |= TTF_STYLE_UNDERLINE;
   if (style.strikethrough)
      styleFlags |= TTF_STYLE_STRIKETHROUGH;

   // Clone a new font from the font type and font size and apply the style flags
   auto it = m_baseFonts.find(style.type);
   if (it == m_baseFonts.end())
   {
      trclog("Could not identify a font asset for type " << style.type << "!");
      return nullptr;
   }
   std::string const& path = it->second;
   TTFFont font(TTF_OpenFont(path.c_str(), fontSize));
   if (font.get() == nullptr)
   {
      trclog("Failed to load font at " << path << "! TTF_OpenFont error: " << TTF_GetError());
      return nullptr;
   }
   TTF_SetFontStyle(font.get(), styleFlags);
   
   // Render the given text with this font
   SDLSurface surface(TTF_RenderText_Solid(font.get(), text.c_str(), SDLUtil::MakeSDLColor(style.color)));
   
   // Return the ready-to-use texture
   return std::make_shared<SDLTexture>(SDL_CreateTextureFromSurface(m_pRenderer, surface.get()));
}

std::shared_ptr<SDLTexture> SDLTextFactory::DrawTextNormal(std::string const& text, int fontSize, ColorRGB color)
{
   return DrawText(text, fontSize, {
      FontType::MONOSPACE,
      color,
      false,
      false,
      false,
      false,
   });
}

std::shared_ptr<SDLTexture> SDLTextFactory::DrawTextBold(std::string const& text, int fontSize, ColorRGB color)
{
   return DrawText(text, fontSize, {
      FontType::MONOSPACE,
      color,
      true,
      false,
      false,
      false,
   });
}

std::shared_ptr<SDLTexture> SDLTextFactory::DrawTextItalic(std::string const& text, int fontSize, ColorRGB color)
{
   return DrawText(text, fontSize, {
      FontType::MONOSPACE,
      color,
      false,
      true,
      false,
      false,
   });
}

std::shared_ptr<SDLTexture> SDLTextFactory::DrawTextBoldItalic(std::string const& text, int fontSize, ColorRGB color)
{
   return DrawText(text, fontSize, {
      FontType::MONOSPACE,
      color,
      true,
      true,
      false,
      false,
   });
}
