#include "GameState.hpp"
#include "../Sources/System/Application.hpp"
#include "../Sources/Core/Scene.hpp"

int GameState::Level = 0;
bool GameState::FirstGame = false;
sf::FloatRect GameState::Bounds = sf::FloatRect(96, 64, 832, 672);

GameState::GameState()
	: ke::State()
	, mScene()
	, mView(sf::FloatRect(0.f, 0.f, 1024.f, 768.f))
	, mAI(mScene, GameState::Level)
	, mConfig(getApplication().getResource<ke::Configuration>("gamedata"))
	, mHero(nullptr)
	, mLevel(GameState::Level)
	, mSoldierSelected(-1)
	, mMoney(300)
	, mMoneyTime(sf::Time::Zero)
	, mReturnButton("gui-game")
	, mSettingsButton("gui-game")
	, mMoneyButton("gui-game")
{
	getApplication().getTime().setTimer(sf::seconds(0.1f), []()
	{
		ke::Log::log("Game started");
	});

	// Update the view
	mScene.getView().setSize(mView.getSize());
	mScene.getView().setCenter(mView.getSize() * 0.5f);

	// Create the terrain and hero
	mScene.createActor<Terrain>("terrain", 0);
	mHero = mScene.createActor<Hero>("hero", 0);

	// Update GUI
	mSoldierButtons.push_back(GameButton("gui-game"));
	mSoldierButtons.push_back(GameButton("gui-game"));
	for (std::size_t i = 0; i < mSoldierButtons.size(); i++)
	{
		mSoldierButtons[i].setTextureRect(sf::IntRect(0, 0, 92, 92));
		mSoldierButtons[i].setPosition(sf::Vector2f(0.f, i * 92.f));

		mSoldierSprites.push_back(sf::Sprite(getApplication().getResource<ke::Texture>("soldier-" + ke::toString(i)), sf::IntRect(0, 64, 64, 64)));
		mSoldierSprites[i].setPosition(sf::Vector2f(14.f, i * 92.f + 14.f));

		mSoldierPrices.push_back(mConfig.getPropertyAs<int>("soldier-" + ke::toString(i) + ".price"));
	}
	mReturnButton.setTextureRect(sf::IntRect(368, 0, 92, 92));
	mReturnButton.setPosition(mScene.getView().getSize().x - 92.f, 0.f);
	mSettingsButton.setTextureRect(sf::IntRect(460, 0, 92, 92));
	mSettingsButton.setPosition(mScene.getView().getSize().x - 92.f, 92.f);
	mMoneyButton.setTextureRect(sf::IntRect(0, 0, 92, 92));
	mMoneyButton.setPosition(0.f, mScene.getView().getSize().y - 92.f);
	mMoneyButton.setScale(2.f, 1.f);
	mMoneySprite.setTexture(getApplication().getResource<ke::Texture>("fx"));
	mMoneySprite.setTextureRect(sf::IntRect(192, 0, 32, 32));
	mMoneySprite.setOrigin(sf::Vector2f(16.f, 16.f));
	mMoneySprite.setPosition(184.f - 46.f, mScene.getView().getSize().y - 46.f);
	mMoneyText.setFont(getApplication().getResource<ke::Font>("font"));
	mMoneyText.setString(ke::toString(mMoney));
	mMoneyText.setCharacterSize(20);
	mMoneyText.setFillColor(sf::Color::White);
	mMoneyText.setOutlineThickness(2.f);
	mMoneyText.setOutlineColor(sf::Color::Black);
	mMoneyText.setOrigin(0.f, mMoneyText.getGlobalBounds().height * 0.5f);
	mMoneyText.setPosition(50.f, mScene.getView().getSize().y - 46.f - 4);
	mLevelText.setFont(getApplication().getResource<ke::Font>("font"));
	mLevelText.setString("Level " + ke::toString(mLevel));
	mLevelText.setCharacterSize(40);
	mLevelText.setFillColor(sf::Color::White);
	mLevelText.setOutlineThickness(2.5f);
	mLevelText.setOutlineColor(sf::Color::Black);
	mLevelText.setOrigin(mLevelText.getGlobalBounds().width * 0.5f, 0.f);
	mLevelText.setPosition(mScene.getView().getSize().x * 0.5f, 10.f);

	// TEMP CHEAT
	getApplication().getWindow().setConsoleCommand("moneyadd", [this](const ke::Window::CommandArgs& args)
	{
		mMoney += 500;
	});
}

GameState::~GameState()
{
}

bool GameState::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
	{
		sf::Vector2f p = getApplication().getWindow().getPointerPositionView(mView);
		bool handled = false;

		// Soldier button
		for (std::size_t i = 0; i < mSoldierButtons.size(); i++)
		{
			if (!handled && mSoldierButtons[i].getBounds().contains(p))
			{
				handled = true;
				mSoldierSelected = static_cast<unsigned int>(i);
				for (std::size_t j = 0; j < mSoldierButtons.size(); j++)
				{
					mSoldierButtons[j].setTextureRect(sf::IntRect(0, 0, 92, 92));
				}
				mSoldierButtons[i].setTextureRect(sf::IntRect(92, 0, 92, 92));
			}
		}

		// Hero button
		if (!handled)
		{
			handled = mHero->handleGui(p);
		}

		if (!handled && mReturnButton.getBounds().contains(p))
		{
			toPostGame(0);
			handled = true;
		}
		if (!handled && mSettingsButton.getBounds().contains(p))
		{
			toSettings();
			handled = true;
		}

		// Map
		if (!handled && GameState::Bounds.contains(p) && mSoldierSelected > -1 && mMoney >= mSoldierPrices[mSoldierSelected])
		{
			mMoney -= mSoldierPrices[mSoldierSelected];
			mScene.createActor<Pop>("", 2, mSoldierSelected)->setPosition(p);
			handled = true;
		}
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		mScene.createActor<Pop>("", 1, 0)->setPosition(getApplication().getWindow().getPointerPositionView(mView));
	}
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
	{
		mScene.createActor<Pop>("", 1, 1)->setPosition(getApplication().getWindow().getPointerPositionView(mView));
	}
    return true;
}

bool GameState::update(sf::Time dt)
{
	for (std::size_t i = 0; i < mScene.getActorCount(); i++)
	{
		Entity::Ptr entity = mScene.getActorT<Entity>(i);
		if (entity != nullptr)
		{
			if (entity->isDead() && entity->getId() != "hero")
			{
				int gain = 50 + ke::random(-10, +10); // TODO : Set
				if (entity->getTeam() == 1)
				{
					mMoney += gain;
					mAI.enemyDied();
				}
				entity->onDie(gain);
				entity->remove();
			}
		}
	}

	mMoneyTime += dt;
	if (mMoneyTime >= sf::seconds(1.f))
	{
		mMoneyTime = sf::Time::Zero;
		mMoney++;
	}

	mMoneyText.setString(ke::toString(mMoney));

	mAI.update(dt);

	mScene.update(dt);

	mHero->updateGui(dt);

	if (mHero->isDead())
	{
		toPostGame(1);
	}
	if (mAI.hasLost())
	{
		toPostGame(2);
	}

	getApplication().getWindow().setDebugInfo("HeroLife", ke::toString(mHero->getLife()));
	getApplication().getWindow().setDebugInfo("Enemies", ke::toString(mEnemiesCount));
	getApplication().getWindow().setDebugInfo("Soldiers", ke::toString(mSoldiersCount));

    return true;
}

void GameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
	mScene.render(target);

	target.setView(mView);
	for (std::size_t i = 0; i < mSoldierButtons.size(); i++)
	{
		if (mMoney >= mSoldierPrices[i])
		{
			mSoldierButtons[i].setColor(sf::Color::White);
		}
		else
		{
			mSoldierButtons[i].setColor(sf::Color(128, 128, 128, 176));
		}
		mSoldierButtons[i].render(target);
		target.draw(mSoldierSprites[i]);
	}

	target.draw(mLevelText);

	mHero->renderGui(target);

	mReturnButton.render(target);
	mSettingsButton.render(target);

	mMoneyButton.render(target);
	target.draw(mMoneySprite);
	target.draw(mMoneyText);
}

void GameState::onActivate()
{
}

void GameState::onDeactivate()
{
}

void GameState::toPostGame(std::size_t id)
{
	if (id == 0)
	{
		// TODO : Quitted
	}
	else if (id == 1)
	{
		// TODO : Lost
	}
	else if (id == 2)
	{
		// TODO : Won
	}

	clearStates();
	
	//pushState("PostGameState");

	GameState::Level = mLevel + 1;
	pushState("GameState");
}

void GameState::toSettings()
{
	pushState("SettingsState");
}
