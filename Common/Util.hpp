#ifndef Util_hpp
#define Util_hpp

/**
 * Most boringly named file...serves as a place to put uncategorized utilities
 */

#include <vector>
#include <ostream>
#include <algorithm>

/// Stringifying support (into ostream) for vectors
template <typename T>
std::ostream & operator<< (std::ostream & os, std::vector<T> const& v)
{
   os << "{ ";
   std::for_each(v.begin(), v.end(), [&](T const& t) {
      os << t;
      os << ", ";
   });
   os << " }";
   return os;
}

#endif
