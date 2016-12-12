#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Sources/Core/Actor.hpp"

#include "Lifebar.hpp"

class Entity : public ke::Actor
{
	public:
		typedef std::shared_ptr<Entity> Ptr;

		Entity(ke::Scene& scene, std::size_t type = 0);
		virtual ~Entity();

		void initializeComponents();

		void updateAll(sf::Time dt);

		int getLife() const;
		void inflige(int amount);
		void restore(int amount);
		bool isDead() const;
		bool isAlive() const;
		float getLifePercent() const;

		sf::FloatRect getBounds() const;

		void render(sf::RenderTarget& target);

		virtual std::size_t getTeam() const;
		std::size_t getType() const;

		virtual void slow(float factor, sf::Time slowDuration);

		virtual bool isHero() const;
		virtual bool isGiant() const;

	protected:
		std::size_t mType;

		int mLife;
		int mLifeMax;
		Lifebar mLifeBar;

		float mTimeFactor;
		sf::Time mSlowRemaining;
};

#endif // ENTITY_HPP

