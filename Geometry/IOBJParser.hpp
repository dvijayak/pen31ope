#ifndef IOBJParser_hpp
#define IOBJParser_hpp

#include <string>
#include <memory>

class IOBJParser
{
    IOBJParser (std::string fileName);
    virtual ~IOBJParser () {}

    virtual void Parse () = 0;
};

#endif
