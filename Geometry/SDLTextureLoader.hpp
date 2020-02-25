#ifndef SDLTextureLoader_hpp
#define SDLTextureLoader_hpp

#include "ITextureLoader.hpp"

class SDLTextureLoader : virtual public ITextureLoader
{
public:
   virtual ~SDLTextureLoader () {}

   std::unique_ptr<TextureMap> LoadFromFile (std::string fileName) override;   
};

#endif
