#include "DirectionLightComponent.hpp"
#include "../Core/Scene.hpp"
#include "../Maths/Vector2.hpp"

namespace ke
{

DirectionLightComponent::DirectionLightComponent(Actor& actor)
	: Component(actor)
	, mLight(nullptr)
{
}

DirectionLightComponent::~DirectionLightComponent()
{
	onUnregister();
}

void DirectionLightComponent::onRegister()
{
	if (!isRegistered())
	{
		if (getScene().useLight() && getScene().getLights() != nullptr && mLight == nullptr)
		{
			mLight = getScene().getLights()->createLightDirectionEmission();
		}
		Component::onRegister();
	}
}

void DirectionLightComponent::onUnregister()
{
	if (isRegistered())
	{
		if (getScene().useLight() && getScene().getLights() != nullptr && mLight != nullptr)
		{
			getScene().getLights()->removeLight(mLight);
			mLight = nullptr;
		}
		Component::onUnregister();
	}
}

void DirectionLightComponent::setColor(sf::Color color)
{
	if (mLight != nullptr)
	{
		mLight->setColor(color);
	}
}

sf::Color DirectionLightComponent::getColor() const
{
	if (mLight != nullptr)
	{
		return mLight->getColor();
	}
	return sf::Color();
}

void DirectionLightComponent::setAngle(float angle)
{
	if (mLight != nullptr)
	{
		mLight->setCastAngle(angle);
	}
}

float DirectionLightComponent::getAngle() const
{
	if (mLight != nullptr)
	{
		return mLight->getCastAngle();
	}
	return 0.f;
}

void DirectionLightComponent::setDirection(sf::Vector2f const& vector)
{
	if (mLight != nullptr)
	{
		mLight->setCastDirection(vector);
	}
}

sf::Vector2f DirectionLightComponent::getDirection() const
{
	if (mLight != nullptr)
	{
		return mLight->getCastDirection();
	}
	return sf::Vector2f();
}

void DirectionLightComponent::serialize(Serializer& serializer)
{
	Component::serialize(serializer);

	serializer.save("angle", getAngle());
	serializer.save("color", getColor());
}

bool DirectionLightComponent::deserialize(Serializer& serializer)
{
	if (!Component::deserialize(serializer))
	{
		return false;
	}

	float angle;
	if (!serializer.load("angle", angle))
	{
		getLog() << ke::Log::Warning << ke::Variant("DirectionLightComponent::deserialize : Can't find \"angle\" in ", getId());
		angle = 0.f;
	}
	setAngle(angle);

	sf::Color color;
	if (!serializer.load("color", color))
	{
		getLog() << ke::Log::Warning << ke::Variant("DirectionLightComponent::deserialize : Can't find \"color\" in ", getId());
		color = sf::Color();
	}
	setColor(color);

	return true;
}

} // namespace ke