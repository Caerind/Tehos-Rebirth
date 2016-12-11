#include "IntroState.hpp"
#include "../Sources/System/Application.hpp"

IntroState::IntroState() : ke::State()
{
	mElapsed = sf::Time::Zero;
	// TODO : Load resources on another thread

}

IntroState::~IntroState()
{
}

bool IntroState::handleEvent(const sf::Event& event)
{
    return true;
}

bool IntroState::update(sf::Time dt)
{
    return true;
}

void IntroState::render(sf::RenderTarget& target, sf::RenderStates states)
{
}

void IntroState::toMenu()
{
	clearStates();
	pushState("MenuState");
}

