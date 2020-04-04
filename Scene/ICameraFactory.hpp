#ifndef ICameraFactory_hpp
#define ICameraFactory_hpp

#include <memory>
#include <string>

class Camera;

struct ICameraFactory
{
   virtual ~ICameraFactory () {}

   virtual std::unique_ptr<Camera> MakeFromFile (std::string const& filename) = 0;
};

#endif
