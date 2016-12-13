#include "SettingsState.hpp"
#include "../Sources/System/Application.hpp"

SettingsState::SettingsState() : ke::State()
{
	mWindow.setTexture(getApplication().getResource<ke::Texture>("gui-settings"));
	mWindow.setOrigin(mWindow.getGlobalBounds().width * 0.5f, mWindow.getGlobalBounds().height * 0.5f);
	mWindow.setPosition(640.f, 360.f);

	mMask.setSize(sf::Vector2f(1280.f, 720.f));
	mMask.setFillColor(sf::Color(20, 20, 20, 128));

	mCloseButton = sf::FloatRect(320.f + 465.f, 180.f + 219.f, 110.f, 111.f);
	mSoundButton.setPosition(320.f + 195.f, 180.f + 141.f);
	mMusicButton.setPosition(320.f + 195.f, 180.f + 238.f);
	mSoundButton.setSize(sf::Vector2f(62.f, 62.f));
	mMusicButton.setSize(sf::Vector2f(62.f, 62.f));

	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	mSoundButton.setFillColor((config.getPropertyAs<bool>("game.sound")) ? sf::Color::White : sf::Color(55,55,55));
	mMusicButton.setFillColor((config.getPropertyAs<bool>("game.music")) ? sf::Color::White : sf::Color(55,55,55));
}

SettingsState::~SettingsState()
{
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
	{
		sf::Vector2f p = getApplication().getWindow().getPointerPositionView(getApplication().getWindow().getView());
		if (mCloseButton.contains(p) || !mWindow.getGlobalBounds().contains(p))
		{
			getApplication().playSound("select");

			popState();
		}
		if (mSoundButton.getGlobalBounds().contains(p))
		{
			ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
			bool v = !config.getPropertyAs<bool>("game.sound");
			config.setProperty("game.sound", v);
			getApplication().setSoundVolume((v) ? 50.f : 0.f);
			mSoundButton.setFillColor((v) ? sf::Color::White : sf::Color(55, 55, 55));

			getApplication().playSound("select");
		}
		if (mMusicButton.getGlobalBounds().contains(p))
		{
			ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
			bool v = !config.getPropertyAs<bool>("game.music");
			config.setProperty("game.music", v);
			getApplication().setMusicVolume((v) ? 40.f : 0.f);
			mMusicButton.setFillColor((v) ? sf::Color::White : sf::Color(55, 55, 55));

			getApplication().playSound("select");
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
	target.draw(mMask);
	target.draw(mWindow);
	target.draw(mSoundButton);
	target.draw(mMusicButton);
}
