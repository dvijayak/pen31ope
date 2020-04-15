#include "Object3D.hpp"

#include <cmath>

void Object3D::ModelMatrix (Matrix4 const& m)
{
   m_modelMatrix = m;
   m_modelMatrixInverse = Inverse_RotationTranslation(m_modelMatrix); // TODO: Will break once we start supporting scaling.
}

void Object3D::Translate (Vector3 const& translation)
{
   ModelMatrix(m_modelMatrix + Matrix4(Matrix4::elements_array_type{
      0, 0, 0, translation.x,
      0, 0, 0, translation.y,
      0, 0, 0, translation.z,
      0, 0, 0, 0
   }));
}

void Object3D::Rotate (float const x, float const y, float const z)
{
   // Note that this is a rotation about the object's origin.
   // Here is the order of transformations to accomplish this:
   // 1. Translate to origin
   // 2. Apply rotation about the point (0, 0, 0)
   // 3. Translate back to object's world position

   float t_x = m_modelMatrix(0,3), t_y = m_modelMatrix(1,3), t_z = m_modelMatrix(2,3);

   ModelMatrix(
      Matrix4(Matrix4::elements_array_type{
         1, 0, 0, t_x,
         0, 1, 0, t_y,
         0, 0, 1, t_z,
         0, 0, 0, 1
      }) *
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
      Matrix4(Matrix4::elements_array_type{
         1, 0, 0, -t_x,
         0, 1, 0, -t_y,
         0, 0, 1, -t_z,
         0, 0, 0, 1
      }) *
      m_modelMatrix);
}