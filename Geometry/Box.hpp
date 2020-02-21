#ifndef Box_hpp
#define Box_hpp

#include "global.hpp"

#include <type_traits>
#include <array>

#include <cassert>

template <typename NumericType, bool yAxisIncreasesDownwards=true>
struct Box2Type
{
   static_assert(std::is_arithmetic<NumericType>::value, "Box expects a numeric data type");

   typedef std::array<NumericType, 2> point_type;

   point_type topLeft, bottomRight;

   Box2Type (point_type const& _topLeft={0, 0}, point_type const& _bottomRight={0, 0})
      : topLeft(_topLeft), bottomRight(_bottomRight)
   {
      if (yAxisIncreasesDownwards)
      {
         assert(topLeft[1] <= bottomRight[1]);
      }
      else
      {
         assert(topLeft[1] >= bottomRight[1]);
      }   
   }

   inline bool Contains (point_type const& point) const
   {
      NumericType x_min = topLeft[0], y_min = topLeft[1];
      NumericType x_max = bottomRight[0], y_max = bottomRight[1];
      NumericType x = point[0], y = point[1];
      return x >= x_min && x <= x_max && y >= y_min && y <= y_max;
   }

   Box2Type<NumericType> Clip (Box2Type<NumericType> const& limit) const
   {
      point_type clippedTopLeft, clippedBottomRight;

      clippedTopLeft[0] = std::max(topLeft[0], limit.topLeft[0]);
      clippedBottomRight[0] = std::min(bottomRight[0], limit.bottomRight[0]);

      if (yAxisIncreasesDownwards)
      {
         clippedTopLeft[1] = std::max(topLeft[1], limit.topLeft[1]);
         clippedBottomRight[1] = std::min(bottomRight[1], limit.bottomRight[1]);
      }
      else
      {
         clippedTopLeft[1] = std::min(topLeft[1], limit.topLeft[1]);
         clippedBottomRight[1] = std::max(bottomRight[1], limit.bottomRight[1]);
      }      

      return Box2Type<NumericType>{clippedTopLeft, clippedBottomRight};
   }
};

typedef Box2Type<uint>  Box2UInt;
typedef Box2Type<int>   Box2Int;
typedef Box2Type<float> Box2;

#endif
