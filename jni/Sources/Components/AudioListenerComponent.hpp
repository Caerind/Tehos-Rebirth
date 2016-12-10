#ifndef KE_AUDIOLISTENERCOMPONENT_HPP
#define KE_AUDIOLISTENERCOMPONENT_HPP

#include <SFML/Audio/Listener.hpp>

#include "../Core/SceneComponent.hpp"

namespace ke
{

class AudioListenerComponent : public SceneComponent
{
    public:
		typedef std::shared_ptr<AudioListenerComponent> Ptr;

		TYPE(AudioListenerComponent)

		AudioListenerComponent(Actor& actor);
		virtual ~AudioListenerComponent();

		void setGlobalVolume(float volume);
		float getGlobalVolume() const;

		void setDirection(float x = 0.f, float y = 0.f, float z = 0.f);
		void setDirection(const sf::Vector3f& dir);
		sf::Vector3f getDirection() const;

		void setUpVector(float x = 0.f, float y = 0.f, float z = 0.f);
		void setUpVector(const sf::Vector3f& up);
		sf::Vector3f getUpVector() const;

		void onTransformNotified();

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);
};

} // namespace ke

#endif // KE_AUDIOLISTENERCOMPONENT_HPP
