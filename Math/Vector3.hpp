#ifndef Vector3_hpp
#define Vector3_hpp

#include <iostream>

class Vector3
{
private:
    float m_x;
    float m_y;
    float m_z;

public:
    Vector3 (float x=0, float y=0, float z=0)
        : m_x(x), m_y(y), m_z(z)
    {}

    Vector3 (Vector3 const& other)
        : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z)
    {}

    Vector3& operator= (Vector3 const& other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
        return *this;
    }

    float x () const { return m_x; }
    float y () const { return m_y; }
    float z () const { return m_z; }

    float Dot (Vector3 const& other) const;

    float Magnitude () const;
    inline float Length () const { return Magnitude(); }
    inline float Norm () const { return Magnitude(); }

    void Normalize ();
};

std::ostream& operator<< (std::ostream& os, Vector3 const& v);

#endif
