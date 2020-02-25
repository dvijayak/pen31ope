#ifndef Mesh_hpp
#define Mesh_hpp

#include <array>
#include <vector>
#include <memory>
#include <string>

#include "Vector.hpp"

/**
 * Triangular mesh of a 3D object
 */
class Mesh
{
public:
    class Triangle
    {
    public:
        class Vertex
        {
            Vector3 m_xyz;
            Vector2 m_uv;
            Vector3 m_normal;

            friend class Mesh;

        public:
            // Sadly, we have to use explicit setters in order to avoid unnecessary copies when referencing members

            Vector3 const& xyz () const { return m_xyz; }
            Vector2 const& uv () const { return m_uv; }
            Vector3 const& normal () const { return m_normal; }
        };

        typedef std::array<Vertex, 3> vertices_type;

    private:
        vertices_type m_vertices;
        Vector3 m_normal;

        friend class Mesh;

    public:
        Triangle (vertices_type const& vertices) : m_vertices(vertices) {}

        Vertex const& operator[] (uint8_t const index) const
        {
            return m_vertices[index];
        }

        Vector3 const& Normal () const { return m_normal; }
    };

    typedef Triangle face_type;
    typedef std::vector<face_type> faces_type;

private:
    faces_type m_faces;

public:
    Mesh () {}
    faces_type const& GetFaces () const { return m_faces; }

    // TODO: Should separate into a MeshLoader interface
    static std::unique_ptr<Mesh> MakeFromOBJ (std::string const& fileName);
};

#endif
