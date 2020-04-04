#include "LuaContext.hpp"

#include <iostream>

LuaContext::LuaContext ()
{
   lua.open_libraries(sol::lib::base, sol::lib::package);
}

bool LuaContext::LoadFromFile (std::string filename)
{
   auto result = lua.script_file(filename, &sol::script_default_on_error);

   bool rc = result.valid();
   m_loaded = rc;
   if (!rc)
   {
      std::cerr << "Failed load lua script " << filename << std::endl;
   #ifndef NDEBUG
      assert(false);
   #endif
   }
   return rc;
}