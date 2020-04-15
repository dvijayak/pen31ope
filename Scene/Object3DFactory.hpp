#ifndef Object3DFactory_hpp
#define Object3DFactory_hpp

#include "Object3D.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "ITextureLoader.hpp"

// TODO: To inherit from IObject3DFactory
class Object3DFactory
{
   std::unordered_map<uint, std::unique_ptr<Object3D>> m_objectMap;

   std::unique_ptr<ITextureLoader> m_pTextureLoader;

   // TODO: This will need to be made thread-safe eventually
   static uint s_appWideNextAvailableObjectId;

public:
   Object3DFactory ();

   /**
    * Attemps to construct the given 3D object. If successful, the object is maintained within the factory
    * and a raw pointer is exposed to the rest of the world to use. Factory is responsible for maintaining
    * object lifetime.
    */
   Object3D* MakeTexturedObject (std::string const& objFileName, std::string const& diffuseTextureFilename="");

   Object3D* operator[] (uint id) const;
};

#endif
