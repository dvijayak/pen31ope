#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <array>
#include <memory>
#include <string>

#include "Vector3.hpp"
#include "IRenderer.hpp"

/**
 * Triangular mesh of a 3D object
 */
class Mesh
{
private:
    typedef std::vector<std::array<Vector3, 3>> FacesT;
    FacesT m_faces;


public:
    Mesh () {}
    FacesT const& GetFaces () const { return m_faces; }

    static std::unique_ptr<Mesh> MakeFromOBJ (std::string const& fileName);
};

#endif
