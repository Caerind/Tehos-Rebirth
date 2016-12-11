#include "GameState.hpp"
#include "../Sources/System/Application.hpp"
#include "../Sources/Core/Scene.hpp"

GameState::GameState()
	: ke::State()
	, mScene()
	, mView(sf::FloatRect(0.f, 0.f, 1024.f, 768.f))
	, mHero(nullptr)
	, mLevelFinished(false)
	, mEnemiesCount(0)
	, mSoldiersCount(0)
	, mSoldierSelected(-1)
	, mReturnButton("gui-game")
	, mSettingsButton("gui-game")
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

	// Create some actors
	mScene.createActor<Pop>("", 1, 0)->setPosition(50, 100);
	mScene.createActor<Pop>("", 1, 0)->setPosition(800, 100);
	mScene.createActor<Pop>("", 1, 0)->setPosition(400, 100);
	mScene.createActor<Pop>("", 2, 0)->setPosition(700, 500);
	mScene.createActor<Pop>("", 2, 0)->setPosition(300, 500);

	// Update GUI
	mSoldierButtons.push_back(GameButton("gui-game"));
	mSoldierButtons.push_back(GameButton("gui-game"));
	for (std::size_t i = 0; i < mSoldierButtons.size(); i++)
	{
		mSoldierButtons[i].setTextureRect(sf::IntRect(0, 0, 92, 92));
		mSoldierButtons[i].setPosition(sf::Vector2f(0.f, i * 92.f));

		mSoldierSprites.push_back(sf::Sprite(getApplication().getResource<ke::Texture>("soldier-" + ke::toString(i)), sf::IntRect(0, 64, 64, 64)));
		mSoldierSprites[i].setPosition(sf::Vector2f(14.f, i * 92.f + 14.f));
	}
	mReturnButton.setTextureRect(sf::IntRect(368, 0, 92, 92));
	mReturnButton.setPosition(mScene.getView().getSize().x - 92.f, 0.f);
	mSettingsButton.setTextureRect(sf::IntRect(460, 0, 92, 92));
	mSettingsButton.setPosition(mScene.getView().getSize().x - 92.f, 92.f);
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
		}

		if (!handled && mSettingsButton.getBounds().contains(p))
		{
			toSettings();
		}

		// Map
		if (!handled && mSoldierSelected != -1)
		{
			mScene.createActor<Pop>("", 2, mSoldierSelected)->setPosition(p);
		}
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		mScene.createActor<Pop>("", 1, 0)->setPosition(getApplication().getWindow().getPointerPositionView(mView));
	}
    return true;
}

bool GameState::update(sf::Time dt)
{
	mEnemiesCount = 0;
	mSoldiersCount = 0;
	for (std::size_t i = 0; i < mScene.getActorCount(); i++)
	{
		Entity::Ptr entity = mScene.getActorT<Entity>(i);
		if (entity != nullptr)
		{
			if (entity->isDead() && entity->getId() != "hero")
			{
				entity->onDie();
				entity->remove();
			}
			if (entity->getTeam() == 1)
			{
				mEnemiesCount++;
			}
			if (entity->getTeam() == 2)
			{
				mSoldiersCount++;
			}
		}
	}

	mScene.update(dt);

	mHero->updateGui(dt);

	if (mHero->isDead())
	{
		toPostGame(1);
	}
	if (mEnemiesCount == 0 && mLevelFinished)
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
		mSoldierButtons[i].render(target);
		target.draw(mSoldierSprites[i]);
	}

	mHero->renderGui(target);

	mReturnButton.render(target);
	mSettingsButton.render(target);
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
	pushState("PostGameState");
}

void GameState::toSettings()
{
	pushState("SettingsState");
}
