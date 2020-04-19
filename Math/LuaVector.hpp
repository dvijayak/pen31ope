#ifndef LuaVector_hpp
#define LuaVector_hpp

#include "Vector.hpp"

#include <string>

#include "sol/sol.hpp"

template <typename Numeric, uint N, typename Handler>
bool sol_lua_check(sol::types<Vector<Numeric, N>>, lua_State * L, int index, Handler && handler, sol::stack::record & tracking) {
   // indices can be negative to count backwards from the top of the stack,
   // rather than the bottom up
   // to deal with this, we adjust the index to
   // its absolute position using the lua_absindex function 
   int absolute_index = lua_absindex(L, index);

   // Check first N indices for being the proper types
   bool success = true;
   int n = absolute_index + N;
   for (int i = absolute_index; success && i < n; ++i)
   {
      success = success && sol::stack::check<Numeric>(L, i, handler);
   }
   
   // we use N slots, each of the previous takes 1
   tracking.use(N);
   return success;
}

template <typename Numeric, uint N, typename Handler>
Vector<Numeric, N> sol_lua_get(sol::types<Vector<Numeric, N>>, lua_State * L, int index, sol::stack::record & tracking) {
   int absolute_index = lua_absindex(L, index);

   Vector<Numeric, N> v;

   // Get the first N elements
   int n = absolute_index + N;
   for (int i = absolute_index; i < n; ++i)
   {
      v[i] = sol::stack::get<Numeric>(L, i);
   }
   
   // we use N slots, each of the previous takes 1
   tracking.use(N);
   return std::move(v);
}

template <typename Numeric, uint N, typename Handler>
int sol_lua_push(sol::types<Vector<Numeric, N>>, lua_State * L, Vector<Numeric, N> const & v) {
   int amount = 0;
   
   for (int i = 0; i < N; ++i)
   {
      amount += sol::stack::push(L, v[i]);
   }

   return amount;
}

#endif
