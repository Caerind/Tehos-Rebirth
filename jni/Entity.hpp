#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Sources/Core/Actor.hpp"

#include "Lifebar.hpp"

class Entity : public ke::Actor
{
	public:
		typedef std::shared_ptr<Entity> Ptr;

		Entity(ke::Scene& scene);
		virtual ~Entity();

		void initializeComponents();

		void updateAll(sf::Time dt);

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

		virtual void onDie(int gain = 0);

		virtual void slow(float factor, sf::Time slowDuration);

	protected:
		ke::SpriteComponent::Ptr mShadow;
		int mLife;
		int mLifeMax;
		Lifebar mLifeBar;

		std::size_t mTimerSlow;
		float mTimeFactor;
};

#endif // ENTITY_HPP

