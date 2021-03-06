#ifndef Color_hpp
#define Color_hpp

#include <cstdint>
#include <cstdlib>

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

typedef uint32_t ColorRGB; // includes alpha channel

class Color
{
public:
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        static constexpr uint32_t R_MASK = 0xff000000;
        static constexpr uint32_t G_MASK = 0x00ff0000;
        static constexpr uint32_t B_MASK = 0x0000ff00;
        static constexpr uint32_t A_MASK = 0x000000ff;
    #else
        static constexpr uint32_t R_MASK = 0x000000ff;
        static constexpr uint32_t G_MASK = 0x0000ff00;
        static constexpr uint32_t B_MASK = 0x00ff0000;
        static constexpr uint32_t A_MASK = 0xff000000;
    #endif

    static constexpr ColorRGB Black         = 0;
    static constexpr ColorRGB White         = 0xFFFFFF00;
    static constexpr ColorRGB Red           = 0xFF000000;
    static constexpr ColorRGB Green         = 0x00FF0000;
    static constexpr ColorRGB Blue          = 0x0000FF00;
    static constexpr ColorRGB Purple        = 0xFF00FF00;
    static constexpr ColorRGB Yellow        = 0xFFFF0000;
    static constexpr ColorRGB Orange        = 0xFFA50000;
    static constexpr ColorRGB Cyan          = 0x00FFFF00;

    inline static ColorRGB Mix (uint8_t const r=0, uint8_t const g=0, uint8_t const b=0)
    {
        return 
            ((r & 0xFF) << 24) +
            ((g & 0xFF) << 16) +
            ((b & 0xFF) << 8) +
            0xFF;
    }

    inline static ColorRGB Mix (float r=1.f, float g=1.f, float b=1.f)
    {
        return 
            ((static_cast<uint8_t>(255.f * r) & 0xFF) << 24) +
            ((static_cast<uint8_t>(255.f * g) & 0xFF) << 16) +
            ((static_cast<uint8_t>(255.f * b) & 0xFF) << 8) +
            0xFF;
    }

    inline static ColorRGB Random ()
    {
        return Mix(uint8_t(rand() % 0xFF), uint8_t(rand() % 0xFF), uint8_t(rand() % 0xFF));
    }

    /**
     * Generate a new shade of the given color corresponding to the given intensity value.
     * @param {ColorRGB} color
     * @param {float} i Must be between 0 and 1 inclusive
     */
    inline static ColorRGB Intensify (ColorRGB color, float i, float gamma=2.2f)
    {
        float c = powf(i, gamma);

        float r = float(color >> 24) * c;
        float g = float((color >> 16) & 0x00FF) * c;
        float b = float((color >> 8) & 0x0000FF) * c;
        return Mix(uint8_t(roundf(r)), uint8_t(roundf(g)), uint8_t(roundf(b)));
    }

    // Converts a color value to an HTML hex string
    inline static std::string str (ColorRGB color)
    {
        std::stringstream ss;
        ss << "#";
        ss << std::hex << std::setfill('0') << std::setw(2);
        ss << (color >> 24);
        ss << std::hex << std::setfill('0') << std::setw(2);
        ss << ((color >> 16) & 0x00FF);
        ss << std::hex << std::setfill('0') << std::setw(2);
        ss << ((color >> 8) & 0x0000FF);
        return ss.str();
    }
};

#endif