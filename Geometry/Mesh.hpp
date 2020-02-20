#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <array>
#include <memory>
#include <string>

#include "Vector3.hpp"
#include "IRenderer.hpp"

/**
 * TODO: Move to its own file
 */
class Triangle
{
public:
    typedef std::array<Vector3, 3> vertices_type;

private:
    vertices_type m_vertices;
    Vector3 m_normal;

    friend class Mesh;

public:
    Triangle (vertices_type const& vertices) : m_vertices(vertices) {}

    Vector3 const& operator[] (uint8_t const index) const
    {
        return m_vertices[index];
    }

    Vector3 const& Normal () const { return m_normal; }
};

/**
 * Triangular mesh of a 3D object
 */
class Mesh
{
public:
    typedef Triangle face_type;
    typedef std::vector<face_type> faces_type;

private:
    faces_type m_faces;

public:
    Mesh () {}
    faces_type const& GetFaces () const { return m_faces; }

    static std::unique_ptr<Mesh> MakeFromOBJ (std::string const& fileName);
};

#endif
