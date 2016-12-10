#include "PointLightComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

PointLightComponent::PointLightComponent(Actor& actor)
	: SceneComponent(actor)
	, mLight(nullptr)
	, mTexture("")
{
}

PointLightComponent::~PointLightComponent()
{
	onUnregister();
}

void PointLightComponent::onRegister()
{
	if (!isRegistered())
	{
		if (getScene().useLight() && getScene().getLights() != nullptr && mLight == nullptr)
		{
			mLight = getScene().getLights()->createLightPointEmission();
		}
		SceneComponent::onRegister();
	}
}

void PointLightComponent::onUnregister()
{
	if (isRegistered())
	{
		if (getScene().useLight() && getScene().getLights() != nullptr && mLight != nullptr)
		{
			getScene().getLights()->removeLight(mLight);
			mLight = nullptr;
		}
		SceneComponent::onUnregister();
	}
}

void PointLightComponent::setTexture(const std::string& texture)
{
	if (getApplication().hasResource(texture) && mLight != nullptr)
	{
		if (getApplication().isResourceLoaded(texture))
		{
			mTexture = texture;
			mLight->setTexture(getApplication().getResource<Texture>(texture));
			mLight->setPosition(getWorldPosition());
		}
	}
}

std::string PointLightComponent::getTexture() const
{
	return mTexture;
}

void PointLightComponent::setOrigin(const sf::Vector2f& origin)
{
	if (mLight != nullptr)
	{
		mLight->setOrigin(origin);
	}
}

void PointLightComponent::setOrigin(float x, float y)
{
	if (mLight != nullptr)
	{
		mLight->setOrigin(sf::Vector2f(x, y));
	}
}

sf::Vector2f PointLightComponent::getOrigin() const
{
	if (mLight != nullptr)
	{
		return mLight->getOrigin();
	}
	return sf::Vector2f();
}

void PointLightComponent::setColor(sf::Color color)
{
	if (mLight != nullptr)
	{
		mLight->setColor(color);
	}
}

sf::Color PointLightComponent::getColor() const
{
	if (mLight != nullptr)
	{
		return mLight->getColor();
	}
	return sf::Color();
}

void PointLightComponent::setIntensity(float intensity)
{
	if (mLight != nullptr)
	{
		mLight->setScale(intensity * sf::Vector2f(1.f, 1.f));
	}
}

float PointLightComponent::getIntensity() const
{
	if (mLight != nullptr)
	{
		return mLight->getScale().x;
	}
	return 1.f;
}

void PointLightComponent::setOn(bool on)
{
	if (mLight != nullptr)
	{
		mLight->setTurnedOn(on);
	}
}

bool PointLightComponent::isOn() const
{
	if (mLight != nullptr)
	{
		return mLight->isTurnedOn();
	}
	return false;
}

void PointLightComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
	serializer.save("texture", getTexture());
	serializer.save("origin", getOrigin());
	serializer.save("color", getColor());
	serializer.save("intensity", getIntensity());
	serializer.save("on", isOn());
}

bool PointLightComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	std::string texture;
	if (!serializer.load("texture", texture))
	{
		getLog() << ke::Log::Warning << ke::Variant("PointLightComponent::deserialize : Can't find \"texture\" in ", getId());
		texture = "";
	}
	setTexture(texture);

	sf::Vector2f origin;
	if (!serializer.load("origin", origin))
	{
		getLog() << ke::Log::Warning << ke::Variant("PointLightComponent::deserialize : Can't find \"origin\" in ", getId());
		origin = sf::Vector2f();
	}
	setOrigin(origin);

	sf::Color color;
	if (!serializer.load("color", color))
	{
		getLog() << ke::Log::Warning << ke::Variant("PointLightComponent::deserialize : Can't find \"color\" in ", getId());
		color = sf::Color();
	}
	setColor(color);

	float intensity;
	if (!serializer.load("intensity", intensity))
	{
		getLog() << ke::Log::Warning << ke::Variant("PointLightComponent::deserialize : Can't find \"intensity\" in ", getId());
		intensity = 1.f;
	}
	setIntensity(intensity);

	bool on;
	if (!serializer.load("on", on))
	{
		getLog() << ke::Log::Warning << ke::Variant("PointLightComponent::deserialize : Can't find \"on\" in ", getId());
		on = true;
	}
	setOn(on);

	return true;
}

void PointLightComponent::onTransformNotified()
{
	if (mLight != nullptr)
	{
		mLight->setPosition(getWorldPosition());
	}
}

} // namespace ke
