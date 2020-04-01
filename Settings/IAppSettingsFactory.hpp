#ifndef IAppSettingsFactory_hpp
#define IAppSettingsFactory_hpp

#include "AppSettings.hpp"

// using pen31ope;

struct IAppSettingsFactory
{
   virtual ~IAppSettingsFactory () {}

   virtual pen31ope::AppSettings::LoadResult ReadFromFile (std::string const& fileName) = 0;
};

#endif
