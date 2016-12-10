#include "NodeComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

NodeComponent::NodeComponent(Actor& actor)
	: SceneComponent(actor)
{
    setRadius(0.f);
    setColor(sf::Color::Transparent);
}

NodeComponent::~NodeComponent()
{
	onUnregister();
}

bool NodeComponent::renderable() const
{
	return true;
}

void NodeComponent::setColor(sf::Color const& color)
{
    mPoint.setFillColor(color);
}

sf::Color NodeComponent::getColor() const
{
    return mPoint.getFillColor();
}

void NodeComponent::setRadius(float radius)
{
    mPoint.setRadius(radius);
    mPoint.setOrigin(radius, radius);
}

float NodeComponent::getRadius() const
{
    return mPoint.getRadius();
}

void NodeComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
	serializer.save("radius", getRadius());
	serializer.save("color", getColor());
}

bool NodeComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	float radius;
	if (!serializer.load("radius", radius))
	{
		getLog() << ke::Log::Warning << ke::Variant("NodeComponent::deserialize : Can't find \"radius\" in ", getId());
		radius = 0.f;
	}
	setRadius(radius);

	sf::Color color;
	if (!serializer.load("color", color))
	{
		getLog() << ke::Log::Warning << ke::Variant("NodeComponent::deserialize : Can't find \"color\" in ", getId());
		color = sf::Color();
	}
	setColor(color);

	return true;
}

void NodeComponent::renderCurrent(sf::RenderTarget& target, sf::RenderStates states)
{
	if (getRadius() > 0.f)
	{
		target.draw(mPoint, states);
	}
}

} // namespace ke