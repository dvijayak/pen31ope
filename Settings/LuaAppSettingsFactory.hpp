#ifndef LuaAppSettingsFactory_hpp
#define LuaAppSettingsFactory_hpp

#include "IAppSettingsFactory.hpp"

#include "LuaContext.hpp"

class LuaAppSettingsFactory : virtual public IAppSettingsFactory
{
   LuaContext _;

public:
   LuaAppSettingsFactory ();
   virtual ~LuaAppSettingsFactory () {}

   pen31ope::AppSettings::LoadResult ReadFromFile (std::string const& fileName) override;
};

#endif
