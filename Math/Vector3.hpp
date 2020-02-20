#ifndef Vector3_hpp
#define Vector3_hpp

#include <iostream>

class Vector3;
typedef Vector3 Vector;

class Vector3
{
private:
    float m_x;
    float m_y;
    float m_z;

public:
    explicit Vector3 (float const x=0, float const y=0, float const z=0)
        : m_x(x), m_y(y), m_z(z)
    {}

    Vector3(Vector3 const& pointFrom, Vector3 const& pointTo);

    float x () const { return m_x; }
    float y () const { return m_y; }
    float z () const { return m_z; }

    /**
     * Flip direction
     */
    Vector3 operator- () const;

    Vector3 operator+ (Vector3 const& other) const;
    Vector3 operator- (Vector3 const& other) const;
    /**
     * Component-wise multiplication: NOT dot product
     */
    Vector3 operator* (Vector3 const& other) const;
    Vector3 operator* (float const scalar) const;

    /**
     * Tip: Dot product of unit vectors will range between -1 and 1, inclusive
     */
    float Dot (Vector3 const& other) const;
    Vector3 Cross (Vector3 const& other) const;

    float Magnitude () const;
    inline float Length () const { return Magnitude(); }
    inline float Norm () const { return Magnitude(); }

    void Normalize (); // modifies in-place
    Vector3 Normalized () const;
};

std::ostream& operator<< (std::ostream& os, Vector3 const& v);

#endif
