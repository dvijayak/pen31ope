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
   os << "{";
   int n = v.size();
   for (int i = 0; i < n; ++i)
      os << v[i] << (i < n-1 ? ", " : "");
   os << "}";
   return os;
}

#include <type_traits>

/// Hash combiner, taken straight from boost::hash_combine.
/// Courtesy of https://stackoverflow.com/a/2595226
template <class T>
inline void hash_combine (size_t& seed, const T& v)
{
   std::hash<T> hasher;
   seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

#endif
