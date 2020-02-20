#include "Vector3.hpp"

#include <cmath>

std::ostream& operator<< (std::ostream& os, Vector3 const& v)
{
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << "|" << v.Magnitude() << ")";
    return os;
}

Vector3 Vector3::Up         = Vector3( 0,  1,  0);
Vector3 Vector3::Down       = Vector3( 0, -1,  0);
Vector3 Vector3::Left       = Vector3( 1,  0,  0);
Vector3 Vector3::Right      = Vector3(-1,  0,  0);
Vector3 Vector3::Forward    = Vector3( 0,  0,  1);
Vector3 Vector3::Backward   = Vector3( 0,  0, -1);

Vector3::Vector3 (Vector3 const& pointFrom, Vector3 const& pointTo)
{
    Vector3 v = pointTo - pointFrom;
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
}

Vector3 Vector3::operator- () const
{
    return *this * -1.f;
}

Vector3 Vector3::operator+ (Vector3 const& other) const
{
    return Vector3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector3 Vector3::operator- (Vector3 const& other) const
{
    return Vector3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

Vector3 Vector3::operator* (Vector3 const& other) const
{
    return Vector3(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z);
}

Vector3 Vector3::operator* (float const scalar) const
{
    return Vector3(m_x * scalar, m_y * scalar, m_z * scalar);   
}

float Vector3::Dot (Vector3 const& other) const
{
    return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z);
}

Vector3 Vector3::Cross (Vector3 const& other) const
{
    /*
     * Definition:
     * P × Q = PyQz − PzQy, PzQx − PxQz, PxQy − PyQx
     */
    return Vector3(
        m_y * other.m_z - m_z * other.m_y,
        m_z * other.m_x - m_x * other.m_z,
        m_x * other.m_y - m_y * other.m_x  
    );
}

float Vector3::Magnitude () const
{
    return sqrtf(Dot(*this));
}

void Vector3::Normalize ()
{
    float norm = Magnitude();
    m_x /= norm;
    m_y /= norm;
    m_z /= norm;
}

Vector3 Vector3::Normalized () const
{
    return *this * (1.f / Magnitude());
}