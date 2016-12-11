#include "Distribution.hpp"

namespace ke
{

namespace Distributions
{

Distribution<int> uniform(int min, int max)
{
    return uniformT(min, max);
}

Distribution<unsigned int> uniform(unsigned int min, unsigned int max)
{
    return uniformT(min, max);
}

Distribution<float> uniform(float min, float max)
{
    return uniformT(min, max);
}

Distribution<sf::Time> uniform(sf::Time min, sf::Time max)
{
    assert(min <= max);

    const float floatMin = min.asSeconds();
    const float floatMax = max.asSeconds();

    return Distribution<sf::Time>([=] () -> sf::Time
    {
        return sf::seconds(random(floatMin, floatMax));
    });
}

Distribution<sf::Vector2f> rect(const sf::Vector2f& center, const sf::Vector2f& halfSize)
{
    return Distribution<sf::Vector2f>([=] () -> sf::Vector2f
    {
        return sf::Vector2f(randomDev(center.x, halfSize.x), randomDev(center.y, halfSize.y));
    });
}

Distribution<sf::Vector2f> rect(const sf::FloatRect& rect)
{
	return Distribution<sf::Vector2f>([=]() -> sf::Vector2f
	{
		return sf::Vector2f(rect.left + random(0.f, rect.width), rect.top + random(0.f, rect.height));
	});
}

Distribution<sf::Vector2f> circle(const sf::Vector2f& center, float radius)
{
    return Distribution<sf::Vector2f>([=] () -> sf::Vector2f
    {
        sf::Vector2f n;
        setLength(n, radius * random(0.f, 1.f));
        setPolarAngle(n, random(0.f, 360.f));
        return center + n;
    });
}

Distribution<sf::Vector2f> deflect(const sf::Vector2f& direction, float maxRotation)
{
    return Distribution<sf::Vector2f>([=] () -> sf::Vector2f
    {
        return rotated(direction, randomDev(0.f, maxRotation));
    });
}

Distribution<sf::Vector2f> project(const sf::Vector2f& direction, float maxRotation, float minVel, float maxVel)
{
	return Distribution<sf::Vector2f>([=]() -> sf::Vector2f
	{
		return rotated(direction * random(minVel, maxVel), randomDev(0.f, maxRotation));
	});
}

Distribution<sf::Color> colorGrade(const sf::Color& color, float min, float max)
{
	return Distribution<sf::Color>([=]() -> sf::Color
	{
		float r = color.r * random(min, max);
		float g = color.g * random(min, max);
		float b = color.b * random(min, max);
		return sf::Color(static_cast<unsigned int>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b));
	});
}

} // namespace Distributions

} // namespace ke