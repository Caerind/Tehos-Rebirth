#ifndef KE_SCENECOMPONENT_HPP
#define KE_SCENECOMPONENT_HPP

#include <vector>

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Component.hpp"

namespace ke
{

class SceneComponent : public Component
{
    public:
		typedef std::shared_ptr<SceneComponent> Ptr;

		TYPE(SceneComponent)

		// Ctor & Dtor
		SceneComponent(Actor& actor);
		virtual ~SceneComponent();

		// Registeration
		virtual void onRegister();
		virtual void onUnregister();

		// Transform
		const sf::Vector2f& getPosition() const;
		void setPosition(sf::Vector2f const& position);
		void setPosition(float x, float y);
		void move(sf::Vector2f const& movement);
		void move(float x, float y);
		float getRotation() const;
		void setRotation(float rotation);
		void rotate(float rotation);
		const sf::Vector2f& getScale() const;
		void setScale(sf::Vector2f const& scale);
		void setScale(float x, float y);
		void scale(sf::Vector2f const& scale);
		void scale(float x, float y);
		const sf::Transform& getTransform() const;
		float getZ() const;
		void setZ(float z);
		void moveZ(float z);

		// World transform
		sf::Vector2f getWorldPosition();
		sf::Transform getWorldTransform();

		// Visibility
		virtual bool renderable() const;
		void setVisible(bool visible);
		bool isVisible() const;
		virtual void render(sf::RenderTarget& target, sf::RenderStates states);

		// Attachment
		void attachComponent(SceneComponent* component);
		void detachComponent(SceneComponent* component);
		std::string getParentId() const;
		bool attachToParent(const std::string& parentId);

		// Serialization
		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
		// Render children
		virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states);
		void renderChildren(sf::RenderTarget& target, sf::RenderStates states);
		static bool sortChildren(SceneComponent* a, SceneComponent* b);

	protected:
		// Transform
		virtual void onTransformChanged();
		virtual void onTransformNotified();
		virtual void onTransformUpdated();

	private:
		sf::Transformable mTransformable;
		sf::Transform mWorldTransform;

		bool mNeedUpdateTransform; ///< Need to update the transform

		float mZ; ///< Z coordinate of the scene component

	protected:
		SceneComponent* mParent; ///< The scene component which is the parent of this scene component

		std::vector<SceneComponent*> mChildren; ///< The children of this scene component

		bool mVisible; ///< Is visible
};

} // namespace ke

#endif // KE_SCENECOMPONENT_HPP
