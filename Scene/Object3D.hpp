#ifndef Object3D_hpp
#define Object3D_hpp

#include "global.hpp"

#include <memory>

#include "Mesh.hpp"

struct TextureData
{
   uint width, height;
   /**
    * Should support 24-bit color values
    */
   std::vector<uint> pixels;
};

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
    * Contains raw pixels corresponding to the diffuse texture, if provided.
    */
   TextureData m_diffuseTextureData;

   /**
    * Must only be constructed and populated by factory
    */
   Object3D () {}

   friend class Object3DFactory;

public:
   uint Id () const { return m_id; }

   /**
    * While the handle to the mesh can't be changed, no limits on modifying the mesh itself are imposed
    */ 
   Mesh* Mesh() const { return m_mesh.get(); }

   TextureData const& DiffuseTexture () const { return m_diffuseTextureData; }
};

#endif
