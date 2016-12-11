#ifndef DEAD_HPP
#define DEAD_HPP

#include "Sources\Core\Actor.hpp"

class Dead : public ke::Actor
{
	public:
		Dead(ke::Scene& scene, std::size_t team, std::size_t type, int gain = 0);
		~Dead();

		void initializeComponents();

		void update(sf::Time dt);

	private:
		ke::AnimatorComponent::Ptr mSprite;
		ke::SpriteComponent::Ptr mMoneySprite;
		ke::TextComponent::Ptr mMoneyText;
		ke::SpriteComponent::Ptr mCrystalSprite;
		ke::TextComponent::Ptr mCrystalText;
		std::size_t mTeam;
		std::size_t mType;
		sf::Time mElapsed;
		int mGain;
		int mCrystalGain;
};

#endif // DEAD_HPP

