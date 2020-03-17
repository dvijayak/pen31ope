#include "Object3D.hpp"

#include <cmath>

void Object3D::Translate (Vector3 const& translation)
{
   m_modelMatrix += Matrix4(Matrix4::elements_array_type{
      0, 0, 0, translation.x,
      0, 0, 0, translation.y,
      0, 0, 0, translation.z,
      0, 0, 0, 0
   });
}

void Object3D::Rotate (float const x, float const y, float const z)
{
   m_modelMatrix = 
      Matrix4(Matrix4::elements_array_type{
         1, 0, 0, 0,
         0, cosf(x), -sinf(x), 0,
         0, sinf(x), cosf(x), 0,
         0, 0, 0, 1
      }) * 
      Matrix4(Matrix4::elements_array_type{
         cosf(y), 0, sinf(y), 0,
         0, 1, 0, 0,
         -sinf(y), 0, cosf(y), 0,
         0, 0, 0, 1
      }) * 
      Matrix4(Matrix4::elements_array_type{
         cosf(z), -sinf(z), 0, 0,
         sinf(z), cosf(z), 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1
      }) * 
      m_modelMatrix;
}