#include "MenuState.hpp"
#include "../Sources/System/Application.hpp"

MenuState::MenuState() : ke::State()
{
}

MenuState::~MenuState()
{
}

bool MenuState::handleEvent(const sf::Event& event)
{
    return true;
}

bool MenuState::update(sf::Time dt)
{
    return true;
}

void MenuState::render(sf::RenderTarget& target, sf::RenderStates states)
{
}