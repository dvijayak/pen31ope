#ifndef Camera_hpp
#define Camera_hpp

#include <cassert>

#include "Constants.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

class Camera
{
   Matrix4 m_viewMatrix;
   Matrix4 m_perspectiveProjectionMatrix;
   Matrix4 m_projectionViewMatrix;
   
   Vector3 m_position;
   Vector3 m_lookAtDirection;

   float m_fov = Constants::PI/4;
   float m_aspectRatio = 1.f;
   float m_zNearPlaneDistance = 0.1f;
   float m_zFarPlaneDistance = 1000.f;

   void UpdatePerspectiveProjectionMatrix ();

public:
   /**
    * Horizontal field of view angle, in radians
    */
   void Fov (float const angle)
   {
      m_fov = angle;
      UpdatePerspectiveProjectionMatrix();
   }

   /**
    * Aspect ratio of the target screen, computed as width/height
    */
   void Aspect (float const aspect=1.f)
   {
      assert(aspect > 0);
      m_aspectRatio = aspect;
      UpdatePerspectiveProjectionMatrix();
   }

   /**
    * Near plane distance from 0...note that this has to be > 0. Note that this is _distance_, and not the raw negative z coordinate
    */
   void Near (float const n=0.1f)
   {
      assert(n > 0);
      m_zNearPlaneDistance = n;
      UpdatePerspectiveProjectionMatrix();
   }

   /**
    * Far plane distance from 0...note that this has to be > near plane distance
    */
   void Far (float const f=1000.f)
   {
      assert(f > 0);
      assert(f > m_zNearPlaneDistance);
      UpdatePerspectiveProjectionMatrix();
   }

   Matrix4 const& ViewMatrix () const { return m_viewMatrix; }
   Matrix4 const& ProjectionViewMatrix () const { return m_projectionViewMatrix; }

   Vector3 const& Position() const { return m_position; }
   Vector3 const& LookAtDirection() const { return m_lookAtDirection; }   

   void LookAt (Vector3 const& position, Vector3 const& center=Vector3(0, 0, 0), Vector3 const& up=Vector3(0, 1, 0));

   void Move (Vector3 const& translation)
   {
      LookAt(m_position + translation);
   }
};

#endif
