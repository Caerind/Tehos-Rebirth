#include "SettingsState.hpp"
#include "../Sources/System/Application.hpp"

sf::FloatRect SettingsState::CloseButton;
// TODO : SETTINGS BUTTON

SettingsState::SettingsState() : ke::State()
{
	mWindow.setTexture(getApplication().getResource<ke::Texture>("gui-settings"));
	mWindow.setOrigin(mWindow.getGlobalBounds().width * 0.5f, mWindow.getGlobalBounds().height * 0.5f);
	mWindow.setPosition(640.f, 360.f);
}

SettingsState::~SettingsState()
{
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
	{
		sf::Vector2f p = getApplication().getWindow().getPointerPosition();
		if (SettingsState::CloseButton.contains(p) || !mWindow.getGlobalBounds().contains(p))
		{
			popState();
		}
		if (mButton.getGlobalBounds().contains(p))
		{
			// TODO : Switch volume
		}
	}
    return false;
}

bool SettingsState::update(sf::Time dt)
{
	return false;
}

void SettingsState::render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mWindow);
	target.draw(mButton);
}
