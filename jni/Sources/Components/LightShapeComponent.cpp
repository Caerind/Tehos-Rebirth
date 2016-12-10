#include "LightShapeComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

LightShapeComponent::LightShapeComponent(Actor& actor)
	: SceneComponent(actor)
	, mPoints()
	, mLightShape(nullptr)
{
}

LightShapeComponent::~LightShapeComponent()
{
	onUnregister();
}

void LightShapeComponent::onRegister()
{
	if (!isRegistered())
	{
		if (getScene().useLight() && getScene().getLights() != nullptr && mLightShape == nullptr)
		{
			mLightShape = getScene().getLights()->createLightShape();
			if (mLightShape != nullptr)
			{
				mLightShape->setTurnedOn(isVisible());
			}
		}
		SceneComponent::onRegister();
	}
}

void LightShapeComponent::onUnregister()
{
	if (isRegistered())
	{
		if (getScene().useLight() && getScene().getLights() != nullptr && mLightShape != nullptr)
		{
			getScene().getLights()->removeShape(mLightShape);
			mLightShape = nullptr;
		}
		SceneComponent::onUnregister();
	}
}

void LightShapeComponent::setPointCount(std::size_t points)
{
	mPoints.resize(points, sf::Vector2f());
	if (mLightShape != nullptr)
	{
		mLightShape->setPointCount(points);
	}
}

std::size_t LightShapeComponent::getPointCount() const
{
	return mPoints.size();
}

void LightShapeComponent::setPoint(std::size_t index, sf::Vector2f const& point)
{
	mPoints[index] = point;
	if (mLightShape != nullptr)
	{
		mLightShape->setPoint(index, point);
	}
}

sf::Vector2f LightShapeComponent::getPoint(std::size_t index) const
{
	return mPoints[index];
}

sf::FloatRect LightShapeComponent::getLocalBounds()
{
	sf::FloatRect bounds;
	if (mLightShape != nullptr)
	{
		for (std::size_t i = 0; i < mPoints.size(); i++)
		{
			sf::Vector2f p = getTransform().transformPoint(mPoints[i]);
			if (i == 0)
			{
				bounds.left = p.x;
				bounds.top = p.y;
				bounds.width = p.x;
				bounds.height = p.y;
			}
			else
			{
				if (p.x < bounds.left)
				{
					bounds.left = p.x;
				}
				if (p.y < bounds.top)
				{
					bounds.top = p.y;
				}
				if (p.x > bounds.width)
				{
					bounds.width = p.x;
				}
				if (p.y > bounds.height)
				{
					bounds.height = p.y;
				}
			}
		}
		bounds.width -= bounds.left;
		bounds.height -= bounds.top;
	}
	return bounds;
}

sf::FloatRect LightShapeComponent::getGlobalBounds()
{
	sf::FloatRect bounds;
	if (mLightShape != nullptr)
	{
		for (std::size_t i = 0; i < mPoints.size(); i++)
		{
			sf::Vector2f p = getWorldTransform().transformPoint(mPoints[i]);
			if (i == 0)
			{
				bounds.left = p.x;
				bounds.top = p.y;
				bounds.width = p.x;
				bounds.height = p.y;
			}
			else
			{
				if (p.x < bounds.left)
				{
					bounds.left = p.x;
				}
				if (p.y < bounds.top)
				{
					bounds.top = p.y;
				}
				if (p.x > bounds.width)
				{
					bounds.width = p.x;
				}
				if (p.y > bounds.height)
				{
					bounds.height = p.y;
				}
			}
		}
		bounds.width -= bounds.left;
		bounds.height -= bounds.top;
	}
	return bounds;
}

void LightShapeComponent::setPoints(const std::vector<sf::Vector2f>& points)
{
	mPoints = points;
	if (mLightShape != nullptr)
	{
		std::size_t count = mPoints.size();
		mLightShape->setPointCount(count);
		for (std::size_t i = 0; i < count; i++)
		{
			mLightShape->setPoint(i, points[i]);
		}
	}
}

std::vector<sf::Vector2f> LightShapeComponent::getPoints() const
{
	return mPoints;
}

void LightShapeComponent::setOn(bool on)
{
	if (mLightShape != nullptr)
	{
		mLightShape->setTurnedOn(on);
	}
}

bool LightShapeComponent::isOn() const
{
	if (mLightShape != nullptr)
	{
		return mLightShape->isTurnedOn();
	}
	return false;
}

void LightShapeComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
	serializer.save("on", isOn());
	serializer.save("points", getPoints());
}

bool LightShapeComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	std::vector<sf::Vector2f> points;
	if (!serializer.load("points", points))
	{
		getLog() << ke::Log::Error << ke::Variant("LightShapeComponent::deserialize : Can't find \"points\" in ", getId());
		return false;
	}
	setPoints(points);

	bool on;
	if (!serializer.load("on", on))
	{
		getLog() << ke::Log::Warning << ke::Variant("PhysicComponent::deserialize : Can't find \"on\" in ", getId());
		on = true;
	}
	setOn(on);

	return true;
}

void LightShapeComponent::onTransformNotified()
{
	if (mLightShape != nullptr)
	{
		mLightShape->setPosition(getWorldPosition());
		mLightShape->setRotation(getRotation());
		mLightShape->setScale(getScale());
	}
}

} // namespace ke