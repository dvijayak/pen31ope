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
    * Transforms from world to model space. The transpose of this matrix can also be used to transform normals
    */
   Matrix4 m_modelMatrixInverse;

   /**
    * Must only be constructed and populated by factory
    */
   Object3D () {}
   friend class Object3DFactory;
   friend class LuaObject3DFactory;

public:
   uint Id () const { return m_id; }

   /**
    * Until necessary, we won't allow the mesh to be modified by client code
    */ 
   Mesh const* Mesh() const { return m_mesh.get(); }

   Material const* Material() const { return m_material.get(); }

   /**
    * Update the model matrix and all dependents
    */
   void ModelMatrix (Matrix4 const&);

   Matrix4 const& ModelMatrix () const { return m_modelMatrix; }
   Matrix4 const& ModelMatrixInverse () const { return m_modelMatrixInverse; }

   void Translate (float const x, float const y, float const z);
   inline void Translate (Vector3 const& translation)
   {
      Translate(translation[0], translation[1], translation[2]);
   }

   /**
    * Rotate's the object **about its center, i.e. model-space origin**.
    * Specify euler angles, in radians
    */
   void Rotate (float const x, float const y, float const z);
   inline void Rotate (Vector3 const& euler)
   {
      Rotate(euler[0], euler[1], euler[2]);
   }
};

#endif
