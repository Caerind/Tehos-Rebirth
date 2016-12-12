#ifndef DEAD_HPP
#define DEAD_HPP

#include "Sources\Core\Actor.hpp"

class Dead : public ke::Actor
{
	public:
		Dead(ke::Scene& scene, std::size_t team, std::size_t type);
		~Dead();

		void initializeComponents();

		void update(sf::Time dt);

	private:
		ke::AnimatorComponent::Ptr mSprite;
		std::size_t mTeam;
		std::size_t mType;
		sf::Time mElapsed;
};

#endif // DEAD_HPP

