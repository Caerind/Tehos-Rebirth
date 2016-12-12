#ifndef HERO_HPP
#define HERO_HPP

#include "Entity.hpp"
#include "GameEffect.hpp"
#include "Lifebar.hpp"

#include "Sources/Components/AnimatorComponent.hpp"

class Hero : public Entity
{
	public:
		typedef std::shared_ptr<Hero> Ptr;

		Hero(ke::Scene& scene);
		~Hero();

		void initializeComponents();
		void update(sf::Time dt);
		std::size_t getTeam() const;

		bool handleGui(const sf::Vector2f& pointer);
		void updateGui(sf::Time dt);
		void renderGui(sf::RenderTarget& target);
		void render(sf::RenderTarget& target);

		bool canCast() const;
		void cast(std::size_t buttonIndex);

		float getCooldownPercent();

		void loadData();

		void castSpell(int spellId);

		bool isHero() const;

	private:
		ke::AnimatorComponent::Ptr mHero;

		sf::Time mCooldown;
		sf::Time mCooldownMax;

		int mSpell1;
		int mSpell2;

		int mHealPower;
		int mDamagePower;
		sf::Time mSlowPower;
		float mShockPower;

		sf::Sprite mHeroSpell1;
		sf::Sprite mHeroSpell2;
		sf::Sprite mHeroCooldown1;
		sf::Sprite mHeroCooldown2;
};

#endif // HERO_HPP
