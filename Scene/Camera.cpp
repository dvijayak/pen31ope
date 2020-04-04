#include "Camera.hpp"

#include <cmath>
#include <iostream>

void Camera::UpdateProjectionViewMatrix ()
{
   m_projectionViewMatrix = m_perspectiveProjectionMatrix * m_viewMatrix;
}

void Camera::UpdatePerspectiveProjectionMatrix ()
{
   /*
      The math here is derived from sections 5.3-5.5 of Lengyel's 3D Math for Game Programming 
      and CG, third edition.
      
      Let Horizontal field of view = theta
      Then, Focal Length, e = 1/tan(theta/2)

      aspect ratio, a = height/width
      This means that for every 1 width, you have a height
      So, bottom plane, b = -a, top plane, t = a
      Thus, left plane, l = -1, right plane, r = 1

      So given aspect ratio, focal length, near plane distance and far plane distance, we can properly compute l, r, b, t:
      Scale rectangle by n/e. Why? Because we want the distance to the near plane to be n, and we know that focal length is e. That is, n = n/e * e.
      Therefore,
         b = -an/e, t = an/e
         l = -n/e, r = n/e

      According to book,

      2n/(r-l)    0        (r+l)/(r-l)    0
      0         2n/(t-b)   (t+b)/(t-b)    0
      0           0       -(f+n)/(f-n)   -2nf/(f-n)
      0           0       -1              0

      Given what we have here,

      r-l = n/e - -n/e = 2n/e
      Then 2n/(r-l) = 2n/2n/e = e
      If r + (-l) = 2n/e, -l = 2n/e - r. Then r+l = r - (-l) = r - (2n/e - r) = 2r - 2n/e
      Then (r+l)/(r-l) = (2r - 2n/e)/2n/e =  ((2re-2n)/e) * e/2n = (2re - 2n)/2n = re/n - 1 = (n/e)*(e/n) - 1 = 0

      t-b = an/e - (-an/e) = 2an/e
      Then 2n/(t-b) = 2n/2an/e = 2n * e/2an = e/a
      If t + (-b) = 2an/e, -b = 2an/e - t. Then t+b = t - (-b) = t - (2an/e - t) = 2t - 2an/e = 2(t - an/e)
      Then (t+b)/(t-b) = 2(t - an/e) / 2an/e = 2(t - an/e) * e/2an = e(t - an/e)/an = (te - an)/an = te/an - 1 = (an/e)*(e/an) - 1 = 0

      Substituting, we have our final perspective projection matrix:

      e     0      0          0
      0    e/a     0          0
      0     0  -(f+n)/(f-n)  -2nf/(f-n)
      0     0     -1          0
   */

   float const focalLength = 1/tanf(m_fov/2);

   float const aspectRatioInverse = 1/m_aspectRatio; // we prefer the aspect ratio expressed as height/width, even though it is provided as width/height

   // Abbreviations
   float const e = focalLength;
   float const a = aspectRatioInverse;
   float const n = m_zNearPlaneDistance;
   float const f = m_zFarPlaneDistance;

   m_perspectiveProjectionMatrix = Matrix4(Matrix4::elements_array_type{
      e, 0, 0, 0,
      0, e/a, 0, 0,
      0, 0, -(f+n)/(f-n), -(2*n*f)/(f-n),
      0, 0, -1, 0   
   });

   UpdateProjectionViewMatrix();
}

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
   
   UpdateProjectionViewMatrix();

   // Update other members
   m_position = position;
   m_lookAtDirection = Normalized(-direction);
}

void Camera::Translate (Vector3 const& translation)
{
   m_viewMatrix *= Matrix4(Matrix4::elements_array_type{
      1, 0, 0, -translation.x,
      0, 1, 0, -translation.y,
      0, 0, 1, -translation.z,
      0, 0, 0,       1
   });

   UpdateProjectionViewMatrix();

   m_position += translation;
   // Direction doesn't change with a translation
}