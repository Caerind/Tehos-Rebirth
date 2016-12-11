#include "PostGameState.hpp"
#include "../Sources/System/Application.hpp"

PostGameState::PostGameState() : ke::State()
{
}

PostGameState::~PostGameState()
{
}

bool PostGameState::handleEvent(const sf::Event& event)
{
    return true;
}

bool PostGameState::update(sf::Time dt)
{
    return true;
}

void PostGameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
}

void PostGameState::onActivate()
{
}

void PostGameState::onDeactivate()
{
}

