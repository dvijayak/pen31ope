#include "Camera.hpp"

void Camera::LookAt (Vector3 const& position, Vector3 const& center, Vector3 const& up)
{
   Vector3 direction = position - center;

   // Compute view space orthonormal basis vectors for this camera
   Vector3 k = Normalized(direction);
   Vector3 i = Normalized(Cross(Normalized(up), k));
   Vector3 j = Normalized(Cross(k, i)); // note the mutative normalization of x, for efficiency

   /*
    * Prepare view transformation matrix
    * 
    * This is the inverse of the change-of-basis matrix that maps vectors in the camera view basis
    * to vectors in the world basis.
    * 
    * To simplify the inverse calculation, we take advantage of this mathematical beauty:
    *    Let M be the 4-by-4 change-of-basis matrix.
    *    We know that it is composed of a rotation R followed by a translation T. So M = T * R.
    *    Then,
    *    M^-1 = (T * R)^-1
    *         = R^-1 * T^-1           [Distributive property of the inverse, see https://chortle.ccsu.edu/VectorLessons/vmch16/vmch16_13.html]
    *
    * Given the above, we just need to find the inverses of the rotation and translation separately and left-multiply.
    * 
    * Starting with rotation R, note that it is an orthogonal matrix by definition of rotation (https://math.stackexchange.com/questions/612936/why-are-orthogonal-matrices-generalizations-of-rotations-and-reflections). (As an extra insight,
    * note that the change-of-basis matrix M is nothing but the basis vectors as columns. Rotating this would lead give
    * you the target frame's basis vectors.)
    * Thus, the inverse of this matrix is simply its transpose. R^-1 = R^T
    * 
    * For the translation T, the inverse is simply the negation of the 3-by-1 translation column vector. (Work it out manually, it's pretty easy to prove)
    * 
    * We thus have a computationally simple way of calculating R^-1 and T^-1. To get M^-1, simply left-multiply the two.
    * Note in particular that it is R^-1 * T^-1 and NOT T^-1 * R^-1, due to the way inverse distribution works.
    */
   Matrix4 inverseOfBasisVectors(Matrix4::elements_array_type{
      i.x, i.y, i.z, 0,
      j.x, j.y, j.z, 0,
      k.x, k.y, k.z, 0,
       0,   0,   0,  1
   });
   Matrix4 inverseOfTranslationFromWorldOrigin(Matrix4::elements_array_type{
      1, 0, 0, -position.x,
      0, 1, 0, -position.y,
      0, 0, 1, -position.z,
      0, 0, 0,      1
   });
   m_viewMatrix = inverseOfBasisVectors * inverseOfTranslationFromWorldOrigin;   

   // Calculate projection matrix
   float projection = -1.f/Magnitude(direction);
   Matrix4 projectionMatrix = Matrix4::Identity();
   projectionMatrix(3, 2) = projection;
   m_projectionViewMatrix = projectionMatrix * m_viewMatrix;

   // Update other members
   m_position = position;
   m_lookAtDirection = Normalized(-direction);
   m_projection = projection;
}