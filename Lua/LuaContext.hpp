#ifndef LuaContext_hpp
#define LuaContext_hpp

#include <vector>
#include <string>
#include <sstream>

#include "LuaCommon.hpp"

/**
 * Every instance of LuaContext has its own state/scope.
 * So you would use different instances when loading independent scripts.
 * For scripts that depend on each other, we could have a single shared context instance
 * that the scripts run within.
 */
struct LuaContext
{
   sol::state lua;

   LuaContext ();
   bool LoadFromFile (std::string filename);

   bool Ready () const { return m_loaded; }

private:
   bool m_loaded = false;
};

#endif
