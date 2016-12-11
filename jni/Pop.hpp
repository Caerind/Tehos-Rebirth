#ifndef POP_HPP
#define POP_HPP

#include "Sources/Core/Actor.hpp"

class Pop : public ke::Actor
{
	public:
		Pop(ke::Scene& scene, std::size_t team, std::size_t type);
		~Pop();

		void initializeComponents();

		void update(sf::Time dt);

	private:
		ke::AnimatorComponent::Ptr mSprite;
		ke::SpriteComponent::Ptr mFx;
		std::size_t mTeam;
		std::size_t mType;
		sf::Time mElapsed;
};

#endif // POP_HPP

