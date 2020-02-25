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
    // TODO: TEMPLATIZE for incl yAxis +-direction 
    static Box2UInt MinimumBoundingBox (Vector3 const& v0, Vector3 const& v1, Vector3 const& v2)
    {
        // Assume yAxis increases downwards
        // TODO: Can't always assume this

        float x_min = std::min(v0.x, std::min(v1.x, v2.x));
        float y_min = std::min(v0.y, std::min(v1.y, v2.y));
        float x_max = std::max(v0.x, std::max(v1.x, v2.x));
        float y_max = std::max(v0.y, std::max(v1.y, v2.y));

        // TODO: Clean this up
        return Box2UInt{std::array<uint, 2>{uint(x_min), uint(y_min)}, std::array<uint, 2>{uint(x_max), uint(y_max)}};
    }

    /**
     * Computes the barycentric coordinates of the given point with respect to the given vertices of a triangle
     */
    // TODO: This function is a hotspot. The many Vector3 constructions isn't good.
    static Vector3 BarycentricCoordinates (Vector3 const& p, Vector3 const& v0, Vector3 const& v1, Vector3 const& v2)
    // static Vector3 BarycentricCoordinates (Vector3 const& p, std::array<Vector3, 3> const&  vertices)
    {
        /*
         * Label as follows:
         *      AB = v0, v1
         *      BC = v1, v2
         *      AC = v0, v2
         *      PA = p, v0   
         */     

        // Calculate cross product of edge vectors
        Vector3 u = Cross(Vector3(
            v1.x - v0.x,
            v2.x - v0.x,
            v0.x - p.x
        ), Vector3(
            v1.y - v0.y,
            v2.y - v0.y,
            v0.y - p.y
        ));

        // Compute the coefficients of the barycentric linear combination
        return Vector3(
            1.f - ((u.x + u.y) / u.z),
            u.x / u.z,
            u.y / u.z
        ); // I don't really understand what's going on here, but this is courtesy of ssloy: https://github.com/ssloy/tinyrenderer/wiki/Lesson-2-Triangle-rasterization-and-back-face-culling
    }
};



#endif
