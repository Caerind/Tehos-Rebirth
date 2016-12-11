#ifndef HERO_HPP
#define HERO_HPP

#include "Entity.hpp"
#include "GameButton.hpp"

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

		bool handleGui(const sf::Vector2f& pointer);
		void updateGui(sf::Time dt);
		void renderGui(sf::RenderTarget& target);

		bool canCast() const;
		void cast(std::size_t buttonIndex);

		float getCooldownPercent();

		void loadData();

		void castSpell(int spellId);

	private:
		ke::AnimatorComponent::Ptr mHero;
		std::size_t mHeroType;

		sf::Time mCooldown;
		sf::Time mCooldownMax;

		int mSpell1;
		int mSpell2;

		GameButton mHeroSpell1;
		GameButton mHeroSpell2;
		sf::Sprite mHeroCooldown1;
		sf::Sprite mHeroCooldown2;
};

#endif // HERO_HPP
