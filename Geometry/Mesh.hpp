#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <memory>
#include <string>

#include "Vector.hpp"
#include "IRenderer.hpp"
#include "Triangle.hpp"

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
