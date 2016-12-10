#include "Trigonometry.hpp"

namespace ke
{

	float pi() { return 3.141592653589793f; }
	float radToDeg(float rad) { return 57.29577951308233f * rad; }
	float degToRad(float deg) { return 0.01745329251994f * deg; }
	float cos(float deg) { return std::cos(degToRad(deg)); }
	float sin(float deg) { return std::sin(degToRad(deg)); }
	float tan(float deg) { return std::tan(degToRad(deg)); }
	float acos(float value) { return radToDeg(std::acos(value)); }
	float asin(float value) { return radToDeg(std::asin(value)); }
	float atan(float value) { return radToDeg(std::atan(value)); }
	float atan2(float valY, float valX) { return radToDeg(std::atan2(valY, valX)); }

} // namespace ke
