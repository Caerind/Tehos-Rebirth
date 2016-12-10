#ifndef KE_ACTOR_HPP
#define KE_ACTOR_HPP

#include <SFML/System/Time.hpp>

#include "../System/Application.hpp"
#include "../System/Serializable.hpp"
#include "../System/Serializer.hpp"
#include "../System/Type.hpp"

#include "Component.hpp"
#include "SceneComponent.hpp"
#include "../Components/Components.hpp"

namespace ke
{

class Scene;
class Actor : public Serializable
{
	public:
		typedef std::shared_ptr<Actor> Ptr;

		TYPE(Actor)

	public:
		// Ctor & Dtor
		Actor(Scene& scene);
		virtual ~Actor();

		// Id management
		const std::string& getId() const;
		void setId(const std::string& id);

		// Garbage managemenent
		void remove();
		bool isMarkedForRemoval() const;

		// Transform
		const sf::Vector2f& getPosition() const;
		void setPosition(const sf::Vector2f& position);
		void setPosition(float x, float y);
		void move(const sf::Vector2f& movement);
		void move(float x, float y);
		float getRotation() const;
		void setRotation(float rotation);
		void rotate(float rotation);
		const sf::Vector2f& getScale() const;
		void setScale(const sf::Vector2f& scale);
		void setScale(float x, float y);
		void scale(const sf::Vector2f& scale);
		void scale(float x, float y);
		const sf::Transform& getTransform();
		float getZ() const;
		void setZ(float z);
		void moveZ(float z);

		// Updatable & Visibility
		bool isUpdatable() const;
		void setUpdatable(bool updatable);
		bool isVisible() const;
		void setVisible(bool visible);

		// Initialization
		virtual void initializePhysic();
		virtual void initializeComponents();
		virtual void initialize();

		// Update & render
		virtual void update(sf::Time dt);
		void updateComponents(sf::Time dt);
		virtual void render(sf::RenderTarget& target);
		void renderComponents(sf::RenderTarget& target);

		// Component management
		template <typename T, typename ... Args>
		std::shared_ptr<T> createComponent(Args&& ... args)
		{
			std::shared_ptr<T> component = std::make_shared<T>(*this, std::forward<Args>(args)...);
			component->setId(ke::decToHex(mComponentIdCounter++));
			component->onRegister(); 
			mComponents.push_back(component);
			return component;
		}
		Component::Ptr createComponentFromFactory(const std::string& type, const std::string& id = "");
		void removeComponent(Component::Ptr component);
		void attachComponent(SceneComponent::Ptr component);
		void attachComponent(SceneComponent* component);
		void detachComponent(SceneComponent::Ptr component);
		void detachComponent(SceneComponent* component);
		std::size_t getComponentCount() const;
		Component::Ptr getComponent(std::size_t index);
		Component::Ptr getComponent(const std::string& id);
		template <typename T>
		std::shared_ptr<T> getComponentT(std::size_t index)
		{
			Component::Ptr c = getComponent(index);
			if (c == nullptr)
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(c);
		}
		template <typename T>
		std::shared_ptr<T> getComponentT(const std::string& id)
		{
			Component::Ptr c = getComponent(id);
			if (c == nullptr)
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(c);
		}

		// Physic
		b2Body* getBody();
		void destroyPhysic();
		void prePhysicUpdate();
		void postPhysicUpdate();
		void desiredImpulseX(float impulse);
		void desiredImpulseY(float impulse);
		sf::Vector2f getVelocity() const;
		void setVelocity(const sf::Vector2f& velocity);
		float getAngularVelocity() const;
		void setAngularVelocity(float angularVelocity);
		b2BodyType getPhysicType() const;
		void setPhysicType(const b2BodyType& type);
		bool isFixedRotation() const;
		void setFixedRotation(bool fixed);
		float getMass() const;
		float getInertia() const;
		float getLinearDamping() const;
		void setLinearDamping(float damping);
		float getAngularDamping() const;
		void setAngularDamping(float damping);
		float getGravityScale() const;
		void setGravityScale(float scale);
		bool isPhysicBullet() const;
		void setPhysicBullet(bool bullet);

		// Getters
		Scene& getScene();
		static Log& getLog();
		static Application& getApplication();

		// Serialization
		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);
		void serializeComponent(Serializer& serializer, Component::Ptr component);
		bool deserializeComponent(Serializer& serializer, Component::Ptr component);
		template <typename T>
		std::shared_ptr<T> deserializeComponent(Serializer& serializer)
		{
			if (serializer.read(T::getStaticType()))
			{
				Component::Ptr component = createComponentFromFactory(T::getStaticType(), "tempDeserialized");
				if (component != nullptr)
				{
					if (!component->deserialize(serializer))
					{
						serializer.end();
						return nullptr;
					}
					serializer.end();
					return std::dynamic_pointer_cast<T>(component);
				}
				serializer.end();
			}
			return nullptr;
		}
		void serializeComponents(Serializer& serializer);
		void deserializeComponents(Serializer& serializer);

	private:
		SceneComponent mRoot; ///< The root for scene components

		std::vector<Component::Ptr> mComponents; ///< All the registered components of this actor

		bool mMarkedForRemoval; ///< Do the Actor need to be removed ?

		std::string mId; ///< The ID of the Actor

		std::size_t mComponentIdCounter; ///< The ID generator for registered components

	protected:
		Scene& mScene; ///< Scene where the actor lives in

		b2Body* mBody; ///< Physic body
};

} // namespace ke

#endif // KE_ACTOR_HPP
