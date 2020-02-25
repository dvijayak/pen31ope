#ifndef Triangle_hpp
#define Triangle_hpp

#include "global.hpp"

#include <array>
#include <map>
#include <algorithm>
// #include <type_traits>

#include "Vector.hpp"
#include "Box.hpp"

class Triangle
{
public:
    typedef std::array<Vector3, 3> vertices_type;
    typedef std::array<Vector2, 3> uvs_type;

private:
    vertices_type m_vertices;
    uvs_type m_vertexUVs;
    Vector3 m_normal;

    friend class Mesh;

public:
    Triangle (vertices_type const& vertices) : m_vertices(vertices) {}

    Vector3 const& operator[] (uint8_t const index) const
    {
        return m_vertices[index];
    }

    Vector3 const& Normal () const { return m_normal; }

    // TODO: REMOVE?
    uvs_type const& UVs () const { return m_vertexUVs; }
};

class TriangleUtil
{
public: 
    // TODO: TEMPLATIZE, incl yAxis +-direction
    // TODO: TEMPLATIZE VECTOR
    static Box2UInt MinimumBoundingBox (std::array<Vector3, 3> const& vertices)
    {
        // TODO: Assume yAxis increases downwards
        float x_min = vertices[0].x, y_min = vertices[0].y;
        float x_max = x_min, y_max = y_min;

        for (auto const& v : vertices)
        {
            if (v.x < x_min)
            {
                x_min = v.x;
            }
            if (v.x > x_max)
            {
                x_max = v.x;
            }
            if (v.y < y_min)
            {
                y_min = v.y;
            }
            if (v.y > y_max)
            {
                y_max = v.y;
            }
        }

        return Box2UInt{std::array<uint, 2>{uint(x_min), uint(y_min)}, std::array<uint, 2>{uint(x_max), uint(y_max)}};
    }

    /**
     * Computes the barycentric coordinates of the given point with respect to the given vertices of a triangle
     */
    // template <typename NumericType>
    // Vector3 BarycentricCoordinates (std::array<NumericType, 3> vertices)
    // TODO: Need Vector2 of UInt???
    // TODO: This function is a hotspot. The many Vector3 constructions isn't good.
    template <typename IndexableTriangle>
    static Vector3 BarycentricCoordinates (Vector3 const& p, IndexableTriangle const& vertices)
    // static Vector3 BarycentricCoordinates (Vector3 const& p, std::array<Vector3, 3> const&  vertices)
    {
        /*
         * Label as follows:
         *      AB = vertices[0], vertices[1]
         *      BC = vertices[1], vertices[2]
         *      AC = vertices[0], vertices[2]
         *      PA = p, vertices[0]   
         */     

        // Calculate cross product of edge vectors
        Vector3 u = Cross(Vector3(
            vertices[1].x - vertices[0].x,
            vertices[2].x - vertices[0].x,
            vertices[0].x - p.x
        ), Vector3(
            vertices[1].y - vertices[0].y,
            vertices[2].y - vertices[0].y,
            vertices[0].y - p.y
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
