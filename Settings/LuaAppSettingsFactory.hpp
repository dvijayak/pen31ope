#ifndef LuaAppSettingsFactory_hpp
#define LuaAppSettingsFactory_hpp

#include "IAppSettingsFactory.hpp"

#include "sol/sol.hpp"

class LuaAppSettingsFactory : virtual public IAppSettingsFactory
{
   sol::state m_lua;

public:
   LuaAppSettingsFactory ();
   virtual ~LuaAppSettingsFactory () {}

   pen31ope::AppSettings::LoadResult ReadFromFile (std::string const& fileName) override;
};

#endif
