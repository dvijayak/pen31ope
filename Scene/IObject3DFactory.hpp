#ifndef IObject3DFactory_hpp
#define IObject3DFactory_hpp

#include <memory>
#include <string>

class Object3D;

struct IObject3DFactory
{
   virtual ~IObject3DFactory () {}

   virtual std::unique_ptr<Object3D> MakeFromFile (std::string const& filename) = 0;
};

#endif
