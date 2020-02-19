#ifndef Interpolate_hpp
#define Interpolate_hpp

float Lerp (float v0, float v1, float t)
{
    return v0 + (v1 - v0) * t;
    // OR
    // return v0 * (1.f - t) + v1 * t;
}

#endif