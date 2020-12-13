#ifndef LuaCommon_hpp
#define LuaCommon_hpp

// Lua is a C library, so we need `extern` in order for it to work well with C++'s name mangling
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "sol/sol.hpp"

#endif