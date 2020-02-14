#ifndef Color_hpp
#define Color_hpp

#include <cstdint>

#include <string>
#include <sstream>
#include <iomanip>

struct ColorRGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class Color
{
public:
	static constexpr ColorRGB Black			= {0 , 0, 0};
	static constexpr ColorRGB White			= {0xff, 0xff, 0xff};
	static constexpr ColorRGB Red 			= {0xff, 0, 0};
	static constexpr ColorRGB Green 		= {0, 0xff, 0};
	static constexpr ColorRGB Blue 			= {0, 0, 0xff};
	static constexpr ColorRGB Purple		= {0xff, 0, 0xff};
	static constexpr ColorRGB Yellow 		= {0xff, 0xff, 0};
	static constexpr ColorRGB Orange 		= {0xff, 0xa5, 0};
	static constexpr ColorRGB Cyan 			= {0, 0xff, 0xff};

	inline static ColorRGB Mix (uint8_t r=0, uint8_t g=0, uint8_t b=0)
	{
		return {r, g, b};
	}

	// Converts a color value to an HTML hex string
	inline static std::string str (ColorRGB color)
	{
		std::stringstream ss;
		ss << "#";
		ss << std::hex << std::setfill('0') << std::setw(2);
		ss << color.r;
		ss << std::hex << std::setfill('0') << std::setw(2);
		ss << color.g;
		ss << std::hex << std::setfill('0') << std::setw(2);
		ss << color.b;
		return ss.str();
	}
};

#endif