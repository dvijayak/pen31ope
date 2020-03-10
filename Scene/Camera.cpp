#include "Camera.hpp"

void Camera::LookAt (Vector3 const& position, Vector3 const& center, Vector3 const& up)
{
   Vector3 direction = position - center;

   // Compute view space orthonormal basis vectors for this camera
   Vector3 z = Normalized(direction);
   Vector3 x = Normalized(Cross(Normalized(up), z));
   Vector3 y = Normalized(Cross(z, x)); // note the mutative normalization of x, for efficiency

   // Prepare view transformation matrix
   // This is the inverse of the change-of-basis matrix that maps vectors in the camera view basis
   // to vectors in the world basis, along with a translation to the position of the camera.
   // Because the view basis is orthonormal, the inverse of the change-of-basis matrix is simply its transpose
   // Note that the change-of-basis matrix is the matrix containing the view basis vectors as columns
   // So the inverse (transpose) is simply the basis vectors as rows      
   Matrix4 inverseOfChangeOfBasis(Matrix4::elements_array_type{
      x.x, x.y, x.z, 0,
      y.x, y.y, y.z, 0,
      z.x, z.y, z.z, 0,
       0,   0,   0,  1
   });
   Matrix4 originTranslation(Matrix4::elements_array_type{
      1, 0, 0, -position.x,
      0, 1, 0, -position.y,
      0, 0, 1, -position.z,
      0, 0, 0,      1
   });
   m_viewMatrix = inverseOfChangeOfBasis * originTranslation;   

   // Calculate projection matrix
   float projection = -1.f/Magnitude(direction);
   Matrix4 projectionMatrix = Matrix4::Identity();
   projectionMatrix(3, 2) = projection;
   m_projectionViewMatrix = projectionMatrix * m_viewMatrix;

   // Update other members
   m_position = position;
   m_lookAtDirection = direction;
   m_projection = projection;
}