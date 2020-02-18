#include "Vector3.hpp"

#include <cmath>

std::ostream& operator<< (std::ostream& os, Vector3 const& v)
{
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << "|" << v.Magnitude() << ")";
    return os;
}

float Vector3::Dot (Vector3 const& other) const
{
    return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z);
}

float Vector3::Magnitude () const
{
    return sqrtf(Dot(*this));
}

void Vector3::Normalize ()
{

}