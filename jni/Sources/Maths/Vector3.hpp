#ifndef KE_VECTOR3_HPP
#define KE_VECTOR3_HPP

#include <cassert>
#include <sstream>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include "Trigonometry.hpp"

namespace ke
{

float distance(sf::Vector3f const& l, sf::Vector3f const& r);

bool isZero(sf::Vector3f const& vector);

//sf::Vector3f polarVector(float length, float angle);

float getLength(sf::Vector3f const& vector);
float getSquaredLength(sf::Vector3f const& vector);
void setLength(sf::Vector3f& vector, float length);

bool isNormalized(sf::Vector3f const& vector);
void normalize(sf::Vector3f& vector);
sf::Vector3f normalized(sf::Vector3f const& vector);
//sf::Vector3f normalized(float polarAngle, float elevationAngle);

float getPolarAngle(sf::Vector3f const& vector);
void setPolarAngle(sf::Vector3f& vector, float angle);

//void rotate(sf::Vector3f& vector, float angle);
//sf::Vector3f rotated(sf::Vector3f const& vector, float angle);

float dotProduct(sf::Vector3f const& l, sf::Vector3f const& r);
sf::Vector3f crossProduct(sf::Vector3f const& l, sf::Vector3f const& r);
sf::Vector3f cwiseProduct(sf::Vector3f const& l, sf::Vector3f const& r);
sf::Vector3f cwiseQuotient(sf::Vector3f const& l, sf::Vector3f const& r);
//sf::Vector3f projectedVector(sf::Vector3f const& vector, sf::Vector3f const& axis);

sf::Vector2f toVector2f(sf::Vector3f const& vector);
std::string toString(sf::Vector3f const& vector);
sf::Vector3f toVector3f(std::string const& str);

sf::Vector3f lerp(sf::Vector3f const& start, sf::Vector3f const& end, float percent);
sf::Vector3f slerp(sf::Vector3f const& start, sf::Vector3f const& end, float percent);
sf::Vector3f nlerp(sf::Vector3f const& start, sf::Vector3f const& end, float percent);

} // namespace ke

#endif // KE_VECTOR3_HPP
