#ifndef LuaCameraFactory_hpp
#define LuaCameraFactory_hpp

#include "ICameraFactory.hpp"

#include "LuaContext.hpp"

class LuaCameraFactory : virtual public ICameraFactory
{
   LuaContext _;

public:
   virtual ~LuaCameraFactory () {}

   std::unique_ptr<Camera> MakeFromFile (std::string const& filename) override;
};

#endif
