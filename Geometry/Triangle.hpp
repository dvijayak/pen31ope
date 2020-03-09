#ifndef Triangle_hpp
#define Triangle_hpp

#include "global.hpp"

#include <array>
#include <map>
#include <algorithm>

#include "Vector.hpp"
#include "Box.hpp"

class TriangleUtil
{
public:
    template <typename Numeric>
    static Box2Type<Numeric> MinimumBoundingBox (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2)
    {
        float x_min = std::min(v0.x, std::min(v1.x, v2.x));
        float y_min = std::min(v0.y, std::min(v1.y, v2.y));
        float x_max = std::max(v0.x, std::max(v1.x, v2.x));
        float y_max = std::max(v0.y, std::max(v1.y, v2.y));
        return Box2Type<Numeric>{Vector<Numeric, 2>(x_min, y_min), Vector<Numeric, 2>(x_max, y_max)};
    }

    /**
     * Computes the barycentric coordinates of the given point with respect to the given vertices of a triangle
     */
    // TODO: This function is a hotspot. The many Vector3 constructions isn't good.
    static Vector3 BarycentricCoordinates (Vector3 const& p, Vector3 const& v0, Vector3 const& v1, Vector3 const& v2)
    {
        /*
         * Label as follows:
         *      AB = v0, v1
         *      BC = v1, v2
         *      AC = v0, v2
         *      PA = p, v0   
         */     

        // Calculate cross product of edge vectors
        float u_x, u_y, u_z;
        Cross(
            v1.x - v0.x,
            v2.x - v0.x,
            v0.x - p.x,
            v1.y - v0.y,
            v2.y - v0.y,
            v0.y - p.y,
            u_x, u_y, u_z // out params
        );

        // Compute the coefficients of the barycentric linear combination
        return Vector3(
            1.f - ((u_x + u_y) / u_z),
            u_x / u_z,
            u_y / u_z
        ); // I don't really understand what's going on here, but this is courtesy of ssloy: https://github.com/ssloy/tinyrenderer/wiki/Lesson-2-Triangle-rasterization-and-back-face-culling
    }
};



#endif
