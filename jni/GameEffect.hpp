#ifndef GAMEEFFECT_HPP
#define GAMEEFFECT_HPP

#include "Sources/Core/Actor.hpp"

class GameEffect : public ke::Actor
{
	public:
		typedef std::shared_ptr<GameEffect> Ptr;

		GameEffect(ke::Scene& scene, std::size_t type);
		~GameEffect();

		void initializeComponents();

		void update(sf::Time dt);

	private:
		std::size_t mType;
		ke::SpriteComponent::Ptr mSprite;
		sf::Time mElapsed;
};

#endif // GAMEEFFECT_HPP

