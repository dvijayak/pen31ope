#ifndef Box_hpp
#define Box_hpp

#include "global.hpp"

#include <type_traits>
#include <cassert>

#include "MathUtil.hpp"
#include "Vector.hpp"

template <typename Numeric>
struct Box2Type
{
   static_assert(std::is_arithmetic<Numeric>::value, "Box expects a numeric data type");

   typedef Vector<Numeric, 2> point_type;

   point_type bottomLeft, topRight;

   Box2Type (point_type const& _bottomLeft={0, 0}, point_type const& _topRight={0, 0})
      : bottomLeft(_bottomLeft), topRight(_topRight)
   {
      assert(bottomLeft.y <= topRight.y);
   }

   inline bool Contains (point_type const& point) const
   {
      return point.x >= bottomLeft.x && point.x <= topRight.x && point.y >= bottomLeft.y && point.y <= topRight.y;
   }

   Box2Type<Numeric> Clip (Box2Type<Numeric> const& limit) const
   {
      point_type bottomLeftClipped, topRightClipped;

      bottomLeftClipped.x = Clamp(bottomLeft.x, limit.bottomLeft.x, limit.topRight.x);
      bottomLeftClipped.y = Clamp(bottomLeft.y, limit.bottomLeft.y, limit.topRight.y);

      topRightClipped.x = Clamp(topRight.x, limit.bottomLeft.x, limit.topRight.x);
      topRightClipped.y = Clamp(topRight.y, limit.bottomLeft.y, limit.topRight.y);

      return {bottomLeftClipped, topRightClipped};
   }
};

typedef Box2Type<uint>  Box2UInt;
typedef Box2Type<int>   Box2Int;
typedef Box2Type<float> Box2;

#endif
