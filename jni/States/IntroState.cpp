#include "IntroState.hpp"
#include "../Sources/System/Application.hpp"
#include "GameState.hpp"

IntroState::IntroState() 
	: ke::State()
	, mElapsed(sf::Time::Zero)
{
	getApplication().getWindow().useBackgroundScaled(getApplication().getAssetsPath() + "background.png");
}

bool IntroState::update(sf::Time dt)
{
	mElapsed += dt;
	if (mElapsed > sf::seconds(1.0f))
	{
		// TODO : LOAD RESOURCES HERE

		GameState::Level = 0;

		clearStates();
		pushState("GameState");
	}
    return true;
}

