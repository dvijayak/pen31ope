#ifndef Constants_hpp
#define Constants_hpp

struct Constants
{
   static constexpr float PI = 3.14159265358979f;

   static float Deg2Rad (float const deg) { return deg * PI/180.f; }
   static float Rad2Deg (float const rad) { return rad * 180.f/PI; }

private:
   Constants () {} // prevent external construction
};

#endif
