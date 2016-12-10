#ifndef KE_VECTOR2_HPP
#define KE_VECTOR2_HPP

#include <cassert>
#include <sstream>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include "Trigonometry.hpp"

namespace ke
{

float distance(sf::Vector2f const& l, sf::Vector2f const& r);

bool isZero(sf::Vector2f const& vector);

sf::Vector2f polarVector(float length, float angle);

float getLength(sf::Vector2f const& vector);
float getSquaredLength(sf::Vector2f const& vector);
void setLength(sf::Vector2f& vector, float length);

bool isNormalized(sf::Vector2f const& vector);
void normalize(sf::Vector2f& vector);
sf::Vector2f normalized(sf::Vector2f const& vector);
sf::Vector2f normalized(float angle);

float getPolarAngle(sf::Vector2f const& vector);
void setPolarAngle(sf::Vector2f& vector, float angle);

void rotate(sf::Vector2f& vector, float angle);
sf::Vector2f rotated(sf::Vector2f const& vector, float angle);

float dotProduct(sf::Vector2f const& l, sf::Vector2f const& r);
float crossProduct(sf::Vector2f const& l, sf::Vector2f const& r);

sf::Vector2f cwiseProduct(sf::Vector2f const& l, sf::Vector2f const& r);
sf::Vector2f cwiseQuotient(sf::Vector2f const& l, sf::Vector2f const& r);
sf::Vector2f projectedVector(sf::Vector2f const& vector, sf::Vector2f const& axis);

sf::Vector3f toVector3f(sf::Vector2f const& vector);

sf::Vector2f lerp(sf::Vector2f const& start, sf::Vector2f const& end, float percent);
sf::Vector2f slerp(sf::Vector2f const& start, sf::Vector2f const& end, float percent);
sf::Vector2f nlerp(sf::Vector2f const& start, sf::Vector2f const& end, float percent);

} // namespace ke

#endif // KE_VECTOR2_HPP
