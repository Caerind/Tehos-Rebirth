#include "ShapeComponent.hpp"

namespace ke
{

ShapeComponent::ShapeComponent(Actor& actor)
	: SceneComponent(actor)
	, mPoints()
	, mShape()
{
	mShape.setFillColor(sf::Color::Transparent);
	mShape.setOutlineColor(sf::Color::Transparent);
}

ShapeComponent::~ShapeComponent()
{
	onUnregister();
}

bool ShapeComponent::renderable() const
{
	return true;
}

void ShapeComponent::setPointCount(std::size_t points)
{
	mPoints.resize(points, sf::Vector2f());
	mShape.setPointCount(points);
}

std::size_t ShapeComponent::getPointCount() const
{
	return mPoints.size();
}

void ShapeComponent::setPoint(std::size_t index, sf::Vector2f const& point)
{
	mPoints[index] = point;
	mShape.setPoint(index, point);
}

sf::Vector2f ShapeComponent::getPoint(std::size_t index) const
{
	return mPoints[index];
}

void ShapeComponent::setOutlineThickness(float thickness)
{
	mShape.setOutlineThickness(thickness);
}

float ShapeComponent::getOutlineThickness() const
{
	return mShape.getOutlineThickness();
}

void ShapeComponent::setOutlineColor(sf::Color const& color)
{
	mShape.setOutlineColor(color);
}

sf::Color ShapeComponent::getOutlineColor() const
{
	return mShape.getOutlineColor();
}

void ShapeComponent::setFillColor(sf::Color const& color)
{
	mShape.setFillColor(color);
}

sf::Color ShapeComponent::getFillColor() const
{
	return mShape.getFillColor();
}

sf::FloatRect ShapeComponent::getLocalBounds()
{
	return getTransform().transformRect(mShape.getLocalBounds());
}

sf::FloatRect ShapeComponent::getGlobalBounds()
{
	return getWorldTransform().transformRect(mShape.getLocalBounds());
}

void ShapeComponent::setPoints(const std::vector<sf::Vector2f>& points)
{
	mPoints = points;
	std::size_t count = points.size();
	mShape.setPointCount(count);
	for (std::size_t i = 0; i < count; i++)
	{
		mShape.setPoint(i, points[i]);
	}
}

std::vector<sf::Vector2f> ShapeComponent::getPoints() const
{
	return mPoints;
}

void ShapeComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
	serializer.save("fillColor", getFillColor());
	serializer.save("outColor", getOutlineColor());
	serializer.save("outThick", getOutlineThickness());
	serializer.save("points", getPoints());
}

bool ShapeComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	sf::Color fillColor;
	if (!serializer.load("fillColor", fillColor))
	{
		getLog() << ke::Log::Warning << ke::Variant("ShapeComponent::deserialize : Can't find \"fillColor\" in ", getId());
		fillColor = sf::Color();
	}
	setFillColor(fillColor);

	sf::Color outColor;
	if (!serializer.load("outColor", outColor))
	{
		getLog() << ke::Log::Warning << ke::Variant("ShapeComponent::deserialize : Can't find \"outColor\" in ", getId());
		outColor = sf::Color();
	}
	setOutlineColor(outColor);

	float outThick;
	if (!serializer.load("outThick", outThick))
	{
		getLog() << ke::Log::Warning << ke::Variant("ShapeComponent::deserialize : Can't find \"outThick\" in ", getId());
		outThick = 0.f;
	}
	setOutlineThickness(outThick);

	std::vector<sf::Vector2f> points;
	if (!serializer.load("points", points))
	{
		getLog() << ke::Log::Error << ke::Variant("ShapeComponent::deserialize : Can't find \"points\" in ", getId());
		return false;
	}
	setPoints(points);

	return true;
}

void ShapeComponent::renderCurrent(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mShape, states);
}

} // namespace ke