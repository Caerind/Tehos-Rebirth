#include "MenuState.hpp"
#include "../Sources/System/Application.hpp"

MenuState::MenuState() : ke::State()
{
	ke::Font& font = getApplication().getResource<ke::Font>("font");
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	
	mCrystalText.setFont(font);
	mCrystalText.setFillColor(sf::Color::White);
	mCrystalText.setOutlineColor(sf::Color::Black);
	mCrystalText.setOutlineThickness(2.f);
	mCrystalText.setCharacterSize(20);

	// TODO : PLACE MENU BUTTONS
	mSettingsButton = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
	mQuitButton = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
	mPlayButton = sf::FloatRect(0.f, 0.f, 0.f, 0.f);

	mSpell1 = config.getPropertyAs<int>("hero.spell-1");
	mSpell2 = config.getPropertyAs<int>("hero.spell-2");
	//mSpell1Sprite.setTexture();
	//mSpell2Sprite.setTexture();
	mSpell1Sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	mSpell2Sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
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