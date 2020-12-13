#ifndef SDLTextFactory_hpp
#define SDLTextFactory_hpp

#include <memory>
#include <string>
#include <unordered_map>

#include "Color.hpp"
#include "SDLCommon.hpp"

#include "SDL_ttf.h"

enum FontType
{
   MONOSPACE
};

struct FontStyle
{
   FontType type;
   ColorRGB color;
   bool bold;
   bool italic;
   bool underline;
   bool strikethrough;
};

class SDLTextFactory
{
public:
   bool ready = false;

   SDLTextFactory(SDL_Renderer* p);
   ~SDLTextFactory();
   void Initialize();
   
   std::shared_ptr<SDLTexture> DrawText(std::string const& text, int fontSize, FontStyle style);

   // Sugar
   std::shared_ptr<SDLTexture> DrawTextNormal(std::string const& text, int fontSize, ColorRGB color);
   std::shared_ptr<SDLTexture> DrawTextBold(std::string const& text, int fontSize, ColorRGB color);
   std::shared_ptr<SDLTexture> DrawTextItalic(std::string const& text, int fontSize, ColorRGB color);
   std::shared_ptr<SDLTexture> DrawTextBoldItalic(std::string const& text, int fontSize, ColorRGB color);

private:
   SDL_Renderer* m_pRenderer = nullptr;

   std::unordered_map<FontType, std::string> m_baseFonts; // these are the assets loaded upon initialization
};

#endif