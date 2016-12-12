#include "PreGameState.hpp"
#include "../Sources/System/Application.hpp"
#include "GameState.hpp"

PreGameState::PreGameState() : ke::State()
{
	mWindow.setTexture(getApplication().getResource<ke::Texture>("gui-pregame"));
	mWindow.setOrigin(mWindow.getGlobalBounds().width * 0.5f, mWindow.getGlobalBounds().height * 0.5f);
	mWindow.setPosition(640.f, 360.f);

	mMask.setSize(sf::Vector2f(1280.f, 720.f));
	mMask.setFillColor(sf::Color(20, 20, 20, 128));

	mPage = 0;
	mMaxLevel = getApplication().getResource<ke::Configuration>("gamedata").getPropertyAs<int>("game.level");

	// TODO : PLACE PREGAME BUTTONS
	mButtonsPerPage = 8;
	mCloseButton = sf::FloatRect(640.f + 0.f, 360.f + 0.f, 0.f, 0.f);
	mLeftArrow = sf::FloatRect(640.f + 0.f, 360.f + 0.f, 0.f, 0.f);
	mRightArrow = sf::FloatRect(640.f + 0.f, 360.f + 0.f, 0.f, 0.f);

	ke::Font& font = getApplication().getResource<ke::Font>("font");
	for (std::size_t i = 0; i < mButtonsPerPage; i++)
	{
		mTexts.push_back(sf::Text());
		mTexts[i].setFont(font);
		mTexts[i].setFillColor(sf::Color::White);
		mTexts[i].setOutlineColor(sf::Color::Black);
		mTexts[i].setOutlineThickness(2.f);
		mTexts[i].setCharacterSize(20);
		mTexts[i].setString("XX");
		mTexts[i].setOrigin(mTexts[i].getGlobalBounds().width * 0.5f, mTexts[i].getGlobalBounds().height * 0.5f);
	}

	updatePage();
}

PreGameState::~PreGameState()
{
}

bool PreGameState::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
	{
		sf::Vector2f p = getApplication().getWindow().getPointerPosition();
		if (mCloseButton.contains(p) || !mWindow.getGlobalBounds().contains(p))
		{
			getApplication().playSound("select");

			popState();
		}
		if (mLeftArrow.contains(p) && mPage > 0)
		{
			getApplication().playSound("select");
			mPage--;
			updatePage();
		}
		if (mRightArrow.contains(p) && mPage < (mMaxLevel / static_cast<int>(mButtonsPerPage)))
		{
			getApplication().playSound("select");
			mPage++;
			updatePage();
		}

		for (std::size_t i = 0; i < mButtonsPerPage; i++)
		{
			sf::FloatRect button = sf::FloatRect();
			// TODO : PLACE PREGAME BUTTONS
			int level = mPage * mButtonsPerPage + i;
			if (button.contains(p) && mMaxLevel >= level)
			{
				getApplication().playSound("select");

				GameState::Level = level;

				clearStates();
				pushState("GameState");
			}
		}
	}
    return false;
}

bool PreGameState::update(sf::Time dt)
{
    return true;
}

void PreGameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mMask);
	target.draw(mWindow);
	for (std::size_t i = 0; i < mTexts.size(); i++)
	{
		target.draw(mTexts[i]);
	}
}

void PreGameState::updatePage()
{
	for (std::size_t i = 0; i < mTexts.size(); i++)
	{
		int level = mPage * mButtonsPerPage + i;
		mTexts[i].setString(ke::toString(level));
		mTexts[i].setOrigin(mTexts[i].getGlobalBounds().width * 0.5f, mTexts[i].getGlobalBounds().height * 0.5f);
		mTexts[i].setFillColor((level <= mMaxLevel) ? sf::Color::White : sf::Color::Black);
		// TODO : PLACE PREGAME BUTTONS
	}
}
