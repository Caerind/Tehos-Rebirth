#include "AudioListenerComponent.hpp"

namespace ke
{

AudioListenerComponent::AudioListenerComponent(Actor& actor)
	: SceneComponent(actor)
{
}

AudioListenerComponent::~AudioListenerComponent()
{
	onUnregister();
}

void AudioListenerComponent::setGlobalVolume(float volume)
{
	sf::Listener::setGlobalVolume(volume);
}

float AudioListenerComponent::getGlobalVolume() const
{
	return sf::Listener::getGlobalVolume();
}

void AudioListenerComponent::setDirection(float x, float y, float z)
{
	sf::Listener::setDirection(x, y, z);
}

void AudioListenerComponent::setDirection(const sf::Vector3f& dir)
{
	sf::Listener::setDirection(dir);
}

sf::Vector3f AudioListenerComponent::getDirection() const
{
	return sf::Listener::getDirection();
}

void AudioListenerComponent::setUpVector(float x, float y, float z)
{
	sf::Listener::setUpVector(x, y, z);
}

void AudioListenerComponent::setUpVector(const sf::Vector3f& up)
{
	sf::Listener::setUpVector(up);
}

sf::Vector3f AudioListenerComponent::getUpVector() const
{
	return sf::Listener::getUpVector();
}

void AudioListenerComponent::onTransformNotified()
{
	sf::Listener::setPosition(ke::toVector3f(getWorldPosition()));
}

void AudioListenerComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
	serializer.save("volume", getGlobalVolume());
	serializer.save("direction", getDirection());
	serializer.save("upvector", getUpVector());
}

bool AudioListenerComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	float volume;
	if (!serializer.load("volume", volume))
	{
		getLog() << ke::Log::Warning << ke::Variant("AudioListenerComponent::deserialize : Can't find \"volume\" in ", getId());
		volume = 100.f;
	}
	setGlobalVolume(volume);

	sf::Vector3f direction;
	if (!serializer.load("direction", direction))
	{
		getLog() << ke::Log::Warning << ke::Variant("AudioListenerComponent::deserialize : Can't find \"direction\" in ", getId());
		direction = sf::Vector3f(0.f, 0.f, -1.f);
	}
	setDirection(direction);

	sf::Vector3f upvector;
	if (!serializer.load("upvector", upvector))
	{
		getLog() << ke::Log::Warning << ke::Variant("AudioListenerComponent::deserialize : Can't find \"upvector\" in ", getId());
		upvector = sf::Vector3f(0.f, 1.f, 0.f);
	}
	setUpVector(upvector);

	return true;
}

} // namespace ke
