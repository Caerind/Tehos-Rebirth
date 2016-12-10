#ifndef KE_TRIGONOMETRY_HPP
#define KE_TRIGONOMETRY_HPP

#include <cmath>

namespace ke
{

	float pi();
	float radToDeg(float rad);
	float degToRad(float deg);
	float cos(float deg);
	float sin(float deg);
	float tan(float deg);
	float acos(float value);
	float asin(float value);
	float atan(float value);
	float atan2(float valY, float valX);

} // namespace ke

#endif // KE_TRIGONOMETRY_HPP
