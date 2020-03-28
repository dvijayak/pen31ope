#ifndef Vector_hpp
#define Vector_hpp

#include "global.hpp"

#include <cassert>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <type_traits>

/**
 * Generic Vector class; primary template
 */
template <typename Numeric, uint N>
class Vector
{   
   static_assert(std::is_arithmetic<Numeric>::value, "Vectors can only contain numeric/arithmetic types, i.e. integers and floating-point numbers");

public:
   typedef std::array<Numeric, N> components_type;

private:
   components_type m_components;

public:
   Vector () : Vector(0) {}

   /**
    * Allow implicit construction from std::array
    */
   Vector (components_type const& components) : m_components(components) {}

   /**
    * Create a vector filled with the given value; must be reimplemented by specializations that wish to support this constructor
    */
   explicit Vector (Numeric const k)
   {
      for (uint i = 0; i < N; ++i)
      {
         m_components[i] = k;
      }
   }

   /**
    * Update all elements of this vector to be the given value; must be reimplemented by specializations that wish to support this assignment
    */
   Vector<Numeric, N>& operator= (Numeric const k)
   {
      for (uint i = 0; i < N; ++i)
      {
         m_components[i] = k;
      }
      return *this;
   }

   /**
    * Generic component read/write access; must be reimplemented by all specializations
    */
   inline Numeric& operator[] (uint const index)
   {
      return m_components[index];
   }
   inline Numeric operator[] (uint const index) const
   {
      return m_components[index];
   }
};

//// Specializations ////

/**
 * 3D Vectors -- our bread and butter
 */
template <typename Numeric>
class Vector<Numeric, 3>
{
   static_assert(std::is_arithmetic<Numeric>::value, "Vectors can only contain numeric/arithmetic types, i.e. integers and floating-point numbers");

public:
   Numeric x, y, z;

   static Vector<Numeric, 3> Up;
   static Vector<Numeric, 3> Down;
   static Vector<Numeric, 3> Left;
   static Vector<Numeric, 3> Right;
   static Vector<Numeric, 3> Forward;
   static Vector<Numeric, 3> Backward;

   explicit Vector (Numeric const _x=0, Numeric const _y=0, Numeric const _z=0)
      : x(_x), y(_y), z(_z)
   {}

   explicit Vector (Numeric const k)
      : x(k), y(k), z(k)
   {}

   // explicit Vector (Vector<Numeric, 2> const& other2D)
   //    : m_x(other2D.x, other2D.y)
   // {}

   Vector<Numeric, 3>& operator= (Numeric const k)
   {
      x = k, y = k, z = k;
      return *this;
   }

   // Reimplemented from primary template
   inline Numeric& operator[] (uint const index)
   {
      static_assert(sizeof(Numeric[3]) == sizeof(Vector<Numeric, 3>), "Vector struct is not laid out in memory as an array; cannot cast struct to array");
      return (&x)[index];
   }
   inline Numeric operator[] (uint const index) const
   {
      static_assert(sizeof(Numeric[3]) == sizeof(Vector<Numeric, 3>), "Vector struct is not laid out in memory as an array; cannot cast struct to array");
      return (&x)[index];
   }
   
   inline Vector<Numeric, 2> xy () const { return Vector<Numeric, 2>(x, y); }
};

template<typename Numeric>
Vector<Numeric, 3> Vector<Numeric, 3>::Up       = Vector<Numeric, 3>( 0,  static_cast<Numeric>(1),  0);
template<typename Numeric>
Vector<Numeric, 3> Vector<Numeric, 3>::Down     = Vector<Numeric, 3>( 0, static_cast<Numeric>(-1),  0);
template<typename Numeric>
Vector<Numeric, 3> Vector<Numeric, 3>::Left     = Vector<Numeric, 3>( static_cast<Numeric>(1),  0,  0);
template<typename Numeric>
Vector<Numeric, 3> Vector<Numeric, 3>::Right    = Vector<Numeric, 3>(static_cast<Numeric>(-1),  0,  0);
template<typename Numeric>
Vector<Numeric, 3> Vector<Numeric, 3>::Forward  = Vector<Numeric, 3>( 0,  0,  static_cast<Numeric>(1));
template<typename Numeric>
Vector<Numeric, 3> Vector<Numeric, 3>::Backward = Vector<Numeric, 3>( 0,  0, static_cast<Numeric>(-1));

/**
 * 2D Vectors -- can't forget 'em
 */
template <typename Numeric>
class Vector<Numeric, 2>
{
   static_assert(std::is_arithmetic<Numeric>::value, "Vectors can only contain numeric/arithmetic types, i.e. integers and floating-point numbers");

public:
   Numeric x, y;

   static Vector<Numeric, 2> Up;
   static Vector<Numeric, 2> Down;
   static Vector<Numeric, 2> Left;
   static Vector<Numeric, 2> Right;

   explicit Vector (Numeric const _x=0, Numeric const _y=0)
      : x(_x), y(_y)
   {}

   explicit Vector (Numeric const k)
      : x(k), y(k)
   {}

   Vector<Numeric, 2>& operator= (Numeric const k)
   {
      x = k, y = k;
      return *this;
   }

   // Reimplemented from primary template
   inline Numeric& operator[] (uint const index)
   {
      static_assert(sizeof(Numeric[2]) == sizeof(Vector<Numeric, 2>), "Vector struct is not laid out in memory as an array; cannot cast struct to array");
      return (&x)[index];
   }
   inline Numeric operator[] (uint const index) const
   {
      static_assert(sizeof(Numeric[2]) == sizeof(Vector<Numeric, 2>), "Vector struct is not laid out in memory as an array; cannot cast struct to array");
      return (&x)[index];
   }
};

template<typename Numeric>
Vector<Numeric, 2> Vector<Numeric, 2>::Up    = Vector<Numeric, 2>( 0,  static_cast<Numeric>(1));
template<typename Numeric>
Vector<Numeric, 2> Vector<Numeric, 2>::Down  = Vector<Numeric, 2>( 0, static_cast<Numeric>(-1));
template<typename Numeric>
Vector<Numeric, 2> Vector<Numeric, 2>::Left  = Vector<Numeric, 2>( static_cast<Numeric>(1),  0);
template<typename Numeric>
Vector<Numeric, 2> Vector<Numeric, 2>::Right = Vector<Numeric, 2>(static_cast<Numeric>(-1),  0);

/**
 * 4D Vectors -- for when you need to think outside the box
 */
template <typename Numeric>
class Vector<Numeric, 4>
{
   static_assert(std::is_arithmetic<Numeric>::value, "Vectors can only contain numeric/arithmetic types, i.e. integers and floating-point numbers");

public:
   Numeric x, y, z, w;

   explicit Vector (Numeric const _x=0, Numeric const _y=0, Numeric const _z = 0, Numeric const _w=0)
      : x(_x), y(_y), z(_z), w(_w)
   {}

   explicit Vector (Numeric const k)
      : x(k), y(k), z(k), w(k)
   {}

   Vector<Numeric, 4>& operator= (Numeric const k)
   {
      x = k, y = k, z = k, w = k;
      return *this;
   }

   // Reimplemented from primary template
   inline Numeric& operator[] (uint const index)
   {
      static_assert(sizeof(Numeric[4]) == sizeof(Vector<Numeric, 4>), "Vector struct is not laid out in memory as an array; cannot cast struct to array");
      return (&x)[index];
   }
   inline Numeric operator[] (uint const index) const
   {
      static_assert(sizeof(Numeric[4]) == sizeof(Vector<Numeric, 4>), "Vector struct is not laid out in memory as an array; cannot cast struct to array");
      return (&x)[index];
   }

   inline Vector<Numeric, 3> xyz () const { return Vector<Numeric, 3>(x, y, z); }
   inline Vector<Numeric, 2> xy () const { return Vector<Numeric, 2>(x, y); }
};

//// Creators, as free functions ////

template <typename Numeric, uint N>
Vector<Numeric, N> ZeroVector ()
{
   Vector<Numeric, N> result;
   for (int i = 0; i < N; ++i)
      result[i] = static_cast<Numeric>(0);
   return result;
}

//// Operations, as free functions ////

/// General ///

/**
 * Flip direction
 */
template <typename Numeric, uint N>
Vector<Numeric, N> operator- (Vector<Numeric, N> const& v)
{
   return v * static_cast<Numeric>(-1);
}

template <typename Numeric, uint N>
Vector<Numeric, N> operator+ (Vector<Numeric, N> const&v, Vector<Numeric, N> const& w)
{
   Vector<Numeric, N> result;
   for (uint i = 0; i < N; ++i)
      result[i] = v[i] + w[i];
   return result;
}

template <typename Numeric, uint N>
Vector<Numeric, N>& operator+= (Vector<Numeric, N> & v, Vector<Numeric, N> const& w)
{
   for (uint i = 0; i < N; ++i)
      v[i] += w[i];
   return v;
}

template <typename Numeric, uint N>
Vector<Numeric, N> operator- (Vector<Numeric, N> const&v, Vector<Numeric, N> const& w)
{
   Vector<Numeric, N> result;
   for (uint i = 0; i < N; ++i)
      result[i] = v[i] - w[i];
   return result;
}

template <typename Numeric, uint N>
Vector<Numeric, N>& operator-= (Vector<Numeric, N> & v, Vector<Numeric, N> const& w)
{
   for (uint i = 0; i < N; ++i)
      v[i] -= w[i];
   return v;
}

/**
 * Component-wise multiplication: NOT dot product
 */
template <typename Numeric, uint N>
Vector<Numeric, N> operator* (Vector<Numeric, N> const&v, Vector<Numeric, N> const& w)
{
   Vector<Numeric, N> result;
   for (uint i = 0; i < N; ++i)
      result[i] = v[i] * w[i];
   return result;
}

template <typename Numeric, uint N>
Vector<Numeric, N>& operator*= (Vector<Numeric, N> & v, Vector<Numeric, N> const& w)
{
   for (uint i = 0; i < N; ++i)
      v[i] *= w[i];
   return v;
}

/**
 * Component-wise division
 */
template <typename Numeric, uint N>
Vector<Numeric, N> operator/ (Vector<Numeric, N> const&v, Vector<Numeric, N> const& w)
{
   Vector<Numeric, N> result;
   for (uint i = 0; i < N; ++i)
      result[i] = v[i] / w[i];
   return result;
}

template <typename Numeric, uint N>
Vector<Numeric, N>& operator/= (Vector<Numeric, N> & v, Vector<Numeric, N> const& w)
{
   for (uint i = 0; i < N; ++i)
      v[i] /= w[i];
   return v;
}

/**
 * Scalar product
 */
template <typename Numeric, uint N>
Vector<Numeric, N> operator* (Vector<Numeric, N> const& v, float const scalar)
{
   Vector<Numeric, N> result;
   for (uint i = 0; i < N; ++i)
      result[i] = static_cast<Numeric>(v[i] * scalar);
   return result;
}

template <typename Numeric, uint N>
Vector<Numeric, N>& operator*= (Vector<Numeric, N> const& v, float const scalar)
{
   for (uint i = 0; i < N; ++i)
      v[i] *= scalar;
   return v;
}

/**
 * Scalar division, i.e. inverse of scalar product
 */
template <typename Numeric, uint N>
Vector<Numeric, N> operator/ (Vector<Numeric, N> const& v, float const scalar)
{
   Vector<Numeric, N> result;
   for (uint i = 0; i < N; ++i)
      result[i] = static_cast<Numeric>(v[i] / scalar);
   return result;
}

template <typename Numeric, uint N>
Vector<Numeric, N>& operator/= (Vector<Numeric, N> & v, float const scalar)
{
   for (uint i = 0; i < N; ++i)
      v[i] /= scalar;
   return v;
}

/**
 * Tip: Dot product of unit vectors will range between -1 and 1, inclusive
 */
template <typename Numeric, uint N, typename ResultNumeric=Numeric>
Numeric Dot (Vector<Numeric, N> const& v, Vector<Numeric, N> const& w)
{
   ResultNumeric dot = 0;
   for (uint i = 0; i < N; ++i)
      dot += (v[i]) * w[i];
   return dot;
}

/**
 * Note: due to forced use of single-precision floating-point sqrt function, the result is bound to have precision errors even if the vector numeric type is of higher precision
 */
template <typename Numeric, uint N, typename ResultNumeric=Numeric>
Numeric Magnitude (Vector<Numeric, N> const& v)
{
   return static_cast<ResultNumeric>(sqrtf(Dot<Numeric, N>(v, v)));
}

template <typename Numeric, uint N>
const auto Length = Magnitude<Numeric, N>;

template <typename Numeric, uint N>
const auto Norm = Magnitude<Numeric, N>;

/// Specific ///

/**
 * By definition, normalized vectors cannot be solely integral, so we only support floating-point normalized vectors
 */
template <uint N>
Vector<float, N> Normalized (Vector<float, N> const& v)
{
   return v / Magnitude<float, N>(v);
}

/**
 * Support in-place Normalize() only for floating point types
 * otherwise we will wrongly lose precision for integral types
 */
template <uint N>
Vector<float, N>& Normalize (Vector<float, N> & v)
{
   return v /= Magnitude<float, N>(v);
}

/**
 * Cross product of two vectors is only defined in 3D
 */
template <typename Numeric>
Vector<Numeric, 3> Cross (Vector<Numeric, 3> const& v, Vector<Numeric, 3> const& w)
{
   /*
   * Definition:
   * P × Q = PyQz − PzQy, PzQx − PxQz, PxQy − PyQx
   */
   return Vector<Numeric, 3>(
      v.y * w.z - v.z * w.y,
      v.z * w.x - v.x * w.z,
      v.x * w.y - v.y * w.x  
   );
}

/**
 * Performance-optimized version that minimizes Vector object constructions
 */
inline void Cross (float const v_x, float const v_y, float const v_z, float const w_x, float const w_y, float const w_z, float & out_x, float & out_y, float & out_z)
{
   out_x = v_y * w_z - v_z * w_y;
   out_y = v_z * w_x - v_x * w_z;
   out_z = v_x * w_y - v_y * w_x;
}

//// I/O ////

template <typename Numeric, uint N>
std::ostream& operator<< (std::ostream& os, Vector<Numeric, N> const& v)
{
   os << "(";
   for (uint i = 0; i < N; ++i)
      os << v[i] << (i < N - 1 ? ", " : "");
   os << "|" << v.Magnitude() << ")";
   return os;
}

//// Typedefs ////

typedef Vector<float, 3> Vector3; // by far the most popular
typedef Vector<float, 2> Vector2;
typedef Vector<float, 4> Vector4;

typedef Vector<int, 3> Vector3Int;
typedef Vector<int, 2> Vector2Int;
typedef Vector<int, 4> Vector4Int;

typedef Vector<uint, 3> Vector3UInt;
typedef Vector<uint, 2> Vector2UInt;
typedef Vector<uint, 4> Vector4UInt;

typedef Vector<uint8_t, 3> Vector3Byte;
typedef Vector<uint8_t, 2> Vector2Byte;
typedef Vector<uint8_t, 4> Vector4Byte;

#endif
