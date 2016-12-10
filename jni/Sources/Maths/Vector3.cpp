#include "Vector3.hpp"

namespace ke
{

float distance(sf::Vector3f const& l, sf::Vector3f const& r)
{
    return getLength(l-r);
}

bool isZero(sf::Vector3f const& vector)
{
    return (vector.x == 0.f && vector.y == 0.f && vector.z == 0.f);
}

/*
sf::Vector3f polarVector(float length, float polarAngle, float elevationAngle)
{
    return sf::Vector3f(length * cos(polarAngle), length * sin(polarAngle), length * sin(elevationAngle));
}
*/

float getLength(sf::Vector3f const& vector)
{
    return std::sqrt(getSquaredLength(vector));
}

float getSquaredLength(sf::Vector3f const& vector)
{
    return dotProduct(vector, vector);
}

void setLength(sf::Vector3f& vector, float length)
{
    assert(!isZero(vector));
    vector *= length / getLength(vector);
}

bool isNormalized(sf::Vector3f const& vector)
{
    return (getSquaredLength(vector) == 1.f);
}

void normalize(sf::Vector3f& vector)
{
    setLength(vector, 1.f);
}

sf::Vector3f normalized(sf::Vector3f const& vector)
{
    assert(!isZero(vector));
	return vector / getLength(vector);
}

/*
sf::Vector3f normalized(float polarAngle, float elevationAngle)
{
    return sf::Vector3f(cos(polarAngle), sin(polarAngle), sin(elevationAngle);
}
*/

float getPolarAngle(sf::Vector3f const& vector)
{
    assert(!isZero(vector));
    return atan2(vector.y, vector.x);
}

void setPolarAngle(sf::Vector3f& vector, float polarAngle)
{
    float length = getLength(vector);
    vector.x = length * ke::cos(polarAngle);
    vector.y = length * ke::sin(polarAngle);
}

/*
void rotate(sf::Vector3f& vector, float polarAngle)
{
    float c = ke::cos(polarAngle);
	float s = ke::sin(polarAngle);
    vector = sf::Vector3f(c * vector.x - s * vector.y, s * vector.x + c * vector.y);
}

sf::Vector3f rotated(sf::Vector3f const& vector, float polarAngle)
{
    float c = ke::cos(polarAngle);
	float s = ke::sin(polarAngle);
    return sf::Vector3f(c * vector.x - s * vector.y, s * vector.x + c * vector.y, vector.z);
}
*/

float dotProduct(sf::Vector3f const& l, sf::Vector3f const& r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

sf::Vector3f crossProduct(sf::Vector3f const& l, sf::Vector3f const& r)
{
    return sf::Vector3f(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
}

sf::Vector3f cwiseProduct(sf::Vector3f const& l, sf::Vector3f const& r)
{
	return sf::Vector3f(l.x * r.x, l.y * r.y, l.z * r.z);
}

sf::Vector3f cwiseQuotient(sf::Vector3f const& l, sf::Vector3f const& r)
{
	assert(!isZero(r));
	return sf::Vector3f(l.x / r.x, l.y / r.y, l.z / r.z);
}

/*
sf::Vector3f projectedVector(sf::Vector3f const& vector, sf::Vector3f const& axis)
{
	assert(!isZero(axis));
	return dotProduct(vector, axis) / getSquaredLength(axis) * axis;
}
*/

sf::Vector2f toVector2f(sf::Vector3f const& vector)
{
    return sf::Vector2f(vector.x, vector.y);
}

std::string toString(sf::Vector3f const& vector)
{
    std::ostringstream oss;
    oss << vector.x << "," << vector.y << "," << vector.z;
    return oss.str();
}

sf::Vector3f toVector3f(std::string const& str)
{
    sf::Vector3f vector;
    std::size_t found = str.find_first_of(',');
    if (found != std::string::npos)
    {
        std::istringstream iss(str.substr(0, found));
        iss >> vector.x;
        std::string s = str.substr(found + 1);
        found = s.find_first_of(',');
        if (found != std::string::npos)
        {
            iss.clear();
            iss.str(str.substr(0, found));
            iss >> vector.y;
            iss.clear();
            iss.str(str.substr(found + 1));
            iss >> vector.z;
        }
    }
    return vector;
}

sf::Vector3f lerp(sf::Vector3f const& start, sf::Vector3f const& end, float percent)
{
    return (start + percent * (end - start));
}

sf::Vector3f slerp(sf::Vector3f const& start, sf::Vector3f const& end, float percent)
{
    if (percent == 0.f)
    {
        return start;
    }
    if (start == end || percent == 1.0f)
    {
        return end;
    }
    float theta = ke::acos(dotProduct(start, end) / (getLength(start) * getLength(end)));
    if (theta == 0.f)
    {
        return end;
    }
    float sinTheta = ke::sin(theta);
    return ((ke::sin((1 - percent) * theta) / sinTheta) * start)  + ((ke::sin(percent * theta) / sinTheta) * end);
}

sf::Vector3f nlerp(sf::Vector3f const& start, sf::Vector3f const& end, float percent)
{
    return normalized(lerp(start,end,percent));
}

} // namespace ke