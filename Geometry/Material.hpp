#ifndef Material_hpp
#define Material_hpp

#include <memory>

#include "Texture.hpp"

class Material
{
   std::unique_ptr<TextureMap> m_diffuseMap;

   friend class Object3DFactory;

public:
   TextureMap const* DiffuseMap () const { return m_diffuseMap.get(); }
   void DiffuseMap (std::unique_ptr<TextureMap> diffuseMap) { m_diffuseMap = std::move(diffuseMap); }
};

#endif
