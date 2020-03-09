#ifndef MathUtil_hpp
#define MathUtil_hpp

#include <algorithm>

float Clamp (float const value, float const min, float const max)
{
   return std::min(std::max(value, min), max);
}

#endif
