#ifndef HERO_HPP
#define HERO_HPP

#include "Entity.hpp"

#include "Sources/Components/AnimatorComponent.hpp"
#include "Sources/Components/SpriteComponent.hpp"

class Hero : public Entity
{
	public:
		typedef std::shared_ptr<Hero> Ptr;

		Hero(ke::Scene& scene, std::size_t heroType);
		~Hero();

		void initializeComponents();
		sf::FloatRect getBounds() const;
		void update(sf::Time dt);
		std::size_t getTeam() const;

		std::size_t getHeroType() const;

		bool canCast() const;
		void cast();

		float getCooldownPercent();

		void loadData();

	private:
		ke::AnimatorComponent::Ptr mHero;
		std::size_t mHeroType;

		sf::Time mCooldown;
		sf::Time mCooldownMax;
		
};

#endif // HERO_HPP
