#ifndef Rasterizer_hpp
#define Rasterizer_hpp

#include "IRenderer.hpp"

class Rasterizer
{
private:
   IRenderer* m_pRenderer;

protected:
   IRenderer* GetRenderer () const { return m_pRenderer; }

public:
   Rasterizer (IRenderer* pRenderer=nullptr)
      : m_pRenderer(pRenderer)
   {}

   virtual ~Rasterizer () {}
};

#endif
