#ifndef Color_hpp
#define Color_hpp

#include <cstdint>

#include <string>
#include <sstream>
#include <iomanip>

typedef uint32_t ColorRGB; // includes alpha channel

class Color
{
public:
	static constexpr ColorRGB Black			= 0;
	static constexpr ColorRGB White			= 0xFFFFFF00;
	static constexpr ColorRGB Red 			= 0xFF000000;
	static constexpr ColorRGB Green 			= 0x00FF0000;
	static constexpr ColorRGB Blue 			= 0x0000FF00;
	static constexpr ColorRGB Purple			= 0xFF00FF00;
	static constexpr ColorRGB Yellow 		= 0xFFFF0000;
	static constexpr ColorRGB Orange 		= 0xFFA50000;
	static constexpr ColorRGB Cyan 			= 0x00FFFF00;

	inline static ColorRGB Mix (uint8_t const r=0, uint8_t const g=0, uint8_t const b=0)
	{
		return 
			((r & 0xFF) << 24) +
			((g & 0xFF) << 16) +
			((b & 0xFF) << 8) +
			0xFF;
	}

	inline static ColorRGB Mix (float r=1, float g=1, float b=1)
	{
		return 
			((static_cast<uint8_t>(255.f * r) & 0xFF) << 24) +
			((static_cast<uint8_t>(255.f * g) & 0xFF) << 16) +
			((static_cast<uint8_t>(255.f * b) & 0xFF) << 8) +
			0xFF;
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