#ifndef GAMETEXT_HPP
#define GAMETEXT_HPP

#include "Sources/Core/Actor.hpp"

class GameText : public ke::Actor
{
	public:
		GameText(ke::Scene& scene, int amount, int amount2 = 0);
		~GameText();

		void initializeComponents();

		void update(sf::Time dt);

	private:
		int mGain;
		int mCrystalGain;
		sf::Time mElapsed;
};

#endif // GAMETEXT_HPP