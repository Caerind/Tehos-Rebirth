#include "Component.hpp"

#include "Actor.hpp"
#include "Scene.hpp"

namespace ke
{

Component::Component(Actor& actor)
	: mId("")
	, mRegistered(false)
	, mUpdatable(true)
	, mActor(actor)
{
}

Component::~Component()
{
	onUnregister();
}

std::string Component::getId() const
{
	return mId;
}

void Component::setId(std::string const& id)
{
	mId = id;
}

bool Component::isRegistered() const
{
	return mRegistered;
}

void Component::onRegister()
{
	mRegistered = true;
}

void Component::onUnregister()
{
	mRegistered = false;
}

bool Component::updatable() const
{
	return false;
}

bool Component::isUpdatable() const
{
	return mUpdatable;
}

void Component::setUpdatable(bool updatable)
{
	mUpdatable = updatable;
}

void Component::update(sf::Time dt)
{
}

sf::Vector2f Component::getActorPosition() const
{
	return mActor.getPosition();
}

void Component::setActorPosition(sf::Vector2f const& position)
{
	mActor.setPosition(position);
}

void Component::setActorPosition(float x, float y)
{
	setActorPosition(sf::Vector2f(x, y));
}

void Component::moveActor(sf::Vector2f const& movement)
{
	mActor.move(movement);
}

void Component::moveActor(float x, float y)
{
	moveActor(sf::Vector2f(x, y));
}

float Component::getActorRotation() const
{
	return mActor.getRotation();
}

void Component::setActorRotation(float rotation)
{
	mActor.setRotation(rotation);
}

void Component::rotateActor(float rotation)
{
	mActor.rotate(rotation);
}

sf::Vector2f Component::getActorScale() const
{
	return mActor.getScale();
}

void Component::setActorScale(sf::Vector2f const& scale)
{
	mActor.setScale(scale);
}

void Component::setActorScale(float x, float y)
{
	setActorScale(sf::Vector2f(x, y));
}

void Component::scaleActor(sf::Vector2f const& scale)
{
	mActor.scale(scale);
}

void Component::scaleActor(float x, float y)
{
	scaleActor(sf::Vector2f(x, y));
}

float Component::getActorZ() const
{
	return mActor.getZ();
}

void Component::setActorZ(float z)
{
	mActor.setZ(z);
}

void Component::moveActorZ(float z)
{
	mActor.moveZ(z);
}

Actor& Component::getActor()
{
	return mActor;
}

Scene& Component::getScene()
{
	return mActor.getScene();
}

Log& Component::getLog()
{
	return getApplication().getLog();
}

Application& Component::getApplication()
{
	return Application::instance();
}

void Component::serialize(Serializer& serializer)
{
	serializer.save("id", getId());
	serializer.save("up", isUpdatable());
}

bool Component::deserialize(Serializer& serializer)
{
	if (!serializer.load("id", mId))
	{
		getLog() << ke::Log::Error << "Component::deserialize : Can't find \"id\"";
		return false;
	}

	if (!serializer.load("up", mUpdatable))
	{
		getLog() << ke::Log::Warning << ke::Variant("Component::deserialize : Can't find \"up\" in : ", getId());
		mUpdatable = true;
	}

	return true;
}

} // namespace ke