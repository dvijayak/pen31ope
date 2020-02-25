#include "Object3D.hpp"

#include <cmath>

ColorRGB Object3D::DiffuseColorFromTexture (float const u, float const v) const
{
   uint index = static_cast<uint>(u * m_diffuseTextureData.width) + static_cast<uint>(v * m_diffuseTextureData.height) * m_diffuseTextureData.height;
   return m_diffuseTextureData.pixels[index];
}