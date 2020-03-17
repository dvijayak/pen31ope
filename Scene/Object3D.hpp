#ifndef Object3D_hpp
#define Object3D_hpp

#include "global.hpp"

#include <memory>

#include "Matrix.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Color.hpp"

/**
 * Componentized class for representing objects that will be rendered in a 3D scene.
 */
class Object3D
{
private:

   /**
    * Application-wide unique ID. Only need be guaranteed to be unique during the runtime of the program.
    * We will use a different ID for persistence/serialization across sessions.
    */
   uint m_id;

   /**
    * 3D mesh of the object
    */
   std::unique_ptr<Mesh> m_mesh;

   /**
    * Surface material info for advanced rendering
    */
   std::unique_ptr<Material> m_material;

   /**
    * Transforms from model to world space
    */
   Matrix4 m_modelMatrix;

   /**
    * Must only be constructed and populated by factory
    */
   Object3D () {}
   friend class Object3DFactory;

public:
   uint Id () const { return m_id; }

   /**
    * Until necessary, we won't allow the mesh to be modified by client code
    */ 
   Mesh const* Mesh() const { return m_mesh.get(); }

   Material const* Material() const { return m_material.get(); }

   Matrix4 const& ModelMatrix () const { return m_modelMatrix; }

   void Translate (Vector3 const& translation);

   /**
    * Specify euler angles, in radians
    */
   void Rotate (float const x, float const y, float const z);
};

#endif
