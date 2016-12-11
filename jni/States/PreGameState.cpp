#include "PreGameState.hpp"
#include "../Sources/System/Application.hpp"

PreGameState::PreGameState() : ke::State()
{
}

PreGameState::~PreGameState()
{
}

bool PreGameState::handleEvent(const sf::Event& event)
{
    return true;
}

bool PreGameState::update(sf::Time dt)
{
    return true;
}

void PreGameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
}

void PreGameState::onActivate()
{
}

void PreGameState::onDeactivate()
{
}

