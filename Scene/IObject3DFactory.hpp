#ifndef IObject3DFactory_hpp
#define IObject3DFactory_hpp

#include <memory>
#include <string>
#include <vector>

class Object3D;

struct IObject3DFactory
{
   virtual ~IObject3DFactory () {}

   virtual std::vector<Object3D> MakeFromFile (std::string const& filename) = 0;
};

#endif
