#include "GameState.hpp"
#include "../Sources/System/Application.hpp"
#include "../Sources/Core/Scene.hpp"

GameState::GameState()
	: ke::State()
	, mScene()
	, mView(sf::FloatRect(0.f, 0.f, 1024.f, 768.f))
	, mTerrain(nullptr)
	, mHero(nullptr)
	, mEnemiesCount(0)
	, mSoldiersCount(0)
	, mSoldierSelected(-1)
{
	getApplication().getTime().setTimer(sf::seconds(0.1f), []()
	{
		ke::Log::log("Game started");
	});

	// Update the view
	mScene.getView().setSize(mView.getSize());
	mScene.getView().setCenter(mView.getSize() * 0.5f);

	// Create the terrain
	mTerrain = mScene.createActor<Terrain>("terrain", 0);
	mHero = mScene.createActor<Hero>("hero", 0);

	mScene.createActor<Pop>("", 1, 0)->setPosition(50, 100);
	mScene.createActor<Pop>("", 1, 0)->setPosition(800, 100);
	mScene.createActor<Pop>("", 1, 0)->setPosition(400, 100);

	mScene.createActor<Pop>("", 2, 0)->setPosition(700, 500);
	mScene.createActor<Pop>("", 2, 0)->setPosition(300, 500);

	// Update GUI
	mSoldierButtons.push_back(GameButton("gui-game"));
	for (std::size_t i = 0; i < mSoldierButtons.size(); i++)
	{
		mSoldierButtons[i].setTextureRect(sf::IntRect(0, 0, 92, 92));
		mSoldierButtons[i].setPosition(sf::Vector2f(0.f, i * 92.f));
	}
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

		// Map
		if (!handled)
		{
			mScene.createActor<Pop>("", 2, 0)->setPosition(p);
		}

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
		// TODO : Lost
	}
	if (mEnemiesCount == 0)
	{
		// TODO : Win
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
	}

	mHero->renderGui(target);
}

void GameState::onActivate()
{
}

void GameState::onDeactivate()
{
}
