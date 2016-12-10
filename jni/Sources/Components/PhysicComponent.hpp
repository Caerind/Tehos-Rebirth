#ifndef KE_PHYSICCOMPONENT_HPP
#define KE_PHYSICCOMPONENT_HPP

#include "../ExtLibs/Box2D/Box2D.h"

#include "../Core/SceneComponent.hpp"

namespace ke
{

class PhysicComponent : public SceneComponent
{
	public:
		typedef std::shared_ptr<PhysicComponent> Ptr;

		TYPE(PhysicComponent)

		PhysicComponent(Actor& actor);
		virtual ~PhysicComponent();

		void onRegister();
		void onUnregister();

		void setPointCount(std::size_t points);
		std::size_t getPointCount() const;

		void setPoint(std::size_t index, sf::Vector2f const& point);
		sf::Vector2f getPoint(std::size_t index) const;

		sf::FloatRect getLocalBounds();
		sf::FloatRect getGlobalBounds();

		b2Fixture* getFixture();
		b2Shape* getShape();
		b2PolygonShape* getPolygonShape();

		void setDensity(float density);
		float getDensity() const;

		void setFriction(float friction);
		float getFriction() const;

		void setRestitution(float restitution);
		float getRestitution() const;

		void setSensor(bool sensor);
		bool isSensor() const;

		void setPoints(const std::vector<sf::Vector2f>& points);
		std::vector<sf::Vector2f> getPoints() const;

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
		virtual void onTransformNotified();
		void updatePhysicShape();

	private:
		std::vector<sf::Vector2f> mPoints;
		b2Fixture* mFixture;
};

} // namespace ke

#endif // KE_PHYSICCOMPONENT_HPP
