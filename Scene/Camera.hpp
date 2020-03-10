#ifndef Camera_hpp
#define Camera_hpp

#include "Vector.hpp"
#include "Matrix.hpp"

class Camera
{
   Matrix4 m_viewMatrix;
   Matrix4 m_projectionViewMatrix;
   
   Vector3 m_position;
   Vector3 m_lookAtDirection;
   float m_projection;

public:
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
