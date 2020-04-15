#ifndef LuaObject3DFactory_hpp
#define LuaObject3DFactory_hpp

#include "IObject3DFactory.hpp"

#include "LuaContext.hpp"
#include "ITextureLoader.hpp"

class LuaObject3DFactory : virtual public IObject3DFactory
{
   LuaContext _;
   std::unique_ptr<ITextureLoader> m_pTextureLoader;

public:
   LuaObject3DFactory ();
   virtual ~LuaObject3DFactory () {}

   std::vector<Object3D> MakeFromFile (std::string const& filename);
};

#endif
