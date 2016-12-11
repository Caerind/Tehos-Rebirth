#include "SettingsState.hpp"
#include "../Sources/System/Application.hpp"

SettingsState::SettingsState() : ke::State()
{
}

SettingsState::~SettingsState()
{
}

bool SettingsState::handleEvent(const sf::Event& event)
{
    return true;
}

bool SettingsState::update(sf::Time dt)
{
    return true;
}

void SettingsState::render(sf::RenderTarget& target, sf::RenderStates states)
{
}

void SettingsState::onActivate()
{
}

void SettingsState::onDeactivate()
{
}

