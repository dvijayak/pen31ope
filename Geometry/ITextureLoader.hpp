#ifndef ITextureLoader_hpp
#define ITextureLoader_hpp

#include <memory>
#include <string>

#include "Texture.hpp"

class ITextureLoader
{
public:
   virtual ~ITextureLoader () {}

   virtual std::unique_ptr<TextureMap> LoadFromFile (std::string fileName) = 0;
};

#endif
