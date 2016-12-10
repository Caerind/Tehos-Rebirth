#ifndef KE_COMPONENT_HPP
#define KE_COMPONENT_HPP

#include <memory>
#include <string>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../System/Application.hpp"
#include "../System/Serializable.hpp"
#include "../System/Serializer.hpp"
#include "../System/Type.hpp"

namespace ke
{

class Actor;
class Scene;
class Component : public Serializable
{
    public:
		typedef std::shared_ptr<Component> Ptr;

		TYPE(Component)

		// Ctor & Dtor
		Component(Actor& actor);
		virtual ~Component();

		// Id
		std::string getId() const;
		void setId(std::string const& id);

		// Registeration
		bool isRegistered() const;
		virtual void onRegister(); // If you inherit from this, remember to call Component::onRegister()
		virtual void onUnregister();

		// Update
		virtual bool updatable() const; // Tell if the component class is updatable
		bool isUpdatable() const; // Tell if we need to update this component
		void setUpdatable(bool updatable);
		virtual void update(sf::Time dt);

		// Actor transform
		sf::Vector2f getActorPosition() const;
		void setActorPosition(sf::Vector2f const& position);
		void setActorPosition(float x, float y);
		void moveActor(sf::Vector2f const& movement);
		void moveActor(float x, float y);
		float getActorRotation() const;
		void setActorRotation(float rotation);
		void rotateActor(float rotation);
		sf::Vector2f getActorScale() const;
		void setActorScale(sf::Vector2f const& scale);
		void setActorScale(float x, float y);
		void scaleActor(sf::Vector2f const& scale);
		void scaleActor(float x, float y);
		float getActorZ() const;
		void setActorZ(float z);
		void moveActorZ(float z);

		// Getters
		Actor& getActor();
		Scene& getScene();
		static Log& getLog();
		static Application& getApplication();

		// Serialization
		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
		std::string mId;

		bool mRegistered;

		bool mUpdatable;

	protected:
		Actor& mActor;
};

} // namespace ke

#endif // KE_COMPONENT_HPP
