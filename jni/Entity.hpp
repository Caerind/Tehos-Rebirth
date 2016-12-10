#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Sources/Core/Actor.hpp"

class Entity : public ke::Actor
{
	public:
		typedef std::shared_ptr<Entity> Ptr;

		Entity(ke::Scene& scene);
		virtual ~Entity();

		virtual void serialize(ke::Serializer& serializer);
		virtual bool deserialize(ke::Serializer& serializer);

		int getLife() const;
		void inflige(int amount);
		void restore(int amount);
		bool isDead() const;
		bool isAlive() const;
		float getLifePercent() const;

		virtual sf::FloatRect getBounds() const;

		void render(sf::RenderTarget& target);

		virtual std::size_t getTeam() const;

	protected:
		int mLife;
		int mLifeMax;
};

#endif // ENTITY_HPP

