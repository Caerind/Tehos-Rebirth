#include "GameState.hpp"

int GameState::Level = 0;
bool GameState::FirstGame = false;
sf::FloatRect GameState::Bounds = sf::FloatRect(96, 64, 1088, 624);
sf::FloatRect GameState::MenuButton = sf::FloatRect(780,350,115,70);
sf::FloatRect GameState::NextButton = sf::FloatRect(780,440,115,70);
// TODO : PLACE POST GAME BUTTONS

GameState::GameState()
	: ke::State()
	, mScene(nullptr)
	, mAI(nullptr)
	, mHero(nullptr)
	, mLevel(0)
	, mSoldierSelected(0)
	, mMoney(300)
	, mMoneyTime(sf::Time::Zero)
	, mEnded(false)
{
	newGame(GameState::Level);

	// Create GUI
	ke::Texture& textureGui = getApplication().getResource<ke::Texture>("gui-game");
	ke::Font& font = getApplication().getResource<ke::Font>("font");
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	mSoldierButtons.push_back(sf::Sprite());
	mSoldierButtons.push_back(sf::Sprite());
	mSoldierButtons.push_back(sf::Sprite());
	for (std::size_t i = 0; i < mSoldierButtons.size(); i++)
	{
		mSoldierButtons[i].setTexture(textureGui);
		mSoldierButtons[i].setTextureRect(sf::IntRect(0, 0, 92, 92));
		mSoldierButtons[i].setPosition(sf::Vector2f(92.f * i, mScene->getView().getSize().y - 92.f));

		mSoldierSprites.push_back(sf::Sprite(getApplication().getResource<ke::Texture>("soldier-" + ke::toString(i)), sf::IntRect(0, 64, 64, 64)));
		mSoldierSprites[i].setPosition(sf::Vector2f(92.f * i + 14.f, mScene->getView().getSize().y - 92.f + 14.f));

		mSoldierPrices.push_back(config.getPropertyAs<int>("soldier-" + ke::toString(i) + ".price"));
	}
	mSoldierButtons[mSoldierSelected].setTextureRect(sf::IntRect(92, 0, 92, 92));
	mReturnButton.setTexture(textureGui);
	mReturnButton.setTextureRect(sf::IntRect(368, 0, 92, 92));
	mReturnButton.setPosition(mScene->getView().getSize().x - 92.f, 0.f);
	mSettingsButton.setTexture(textureGui);
	mSettingsButton.setTextureRect(sf::IntRect(460, 0, 92, 92));
	mSettingsButton.setPosition(mScene->getView().getSize().x - 92.f, 92.f);
	mMoneyButton.setTexture(textureGui);
	mMoneyButton.setTextureRect(sf::IntRect(0, 0, 92, 92));
	mMoneyButton.setPosition(0.f, 0.f);
	mMoneyButton.setScale(2.f, 1.f);
	mMoneySprite.setTexture(getApplication().getResource<ke::Texture>("fx"));
	mMoneySprite.setTextureRect(sf::IntRect(192, 0, 32, 32));
	mMoneySprite.setOrigin(sf::Vector2f(16.f, 16.f));
	mMoneySprite.setPosition(184.f - 46.f,  46.f);
	mMoneyText.setFont(font);
	mMoneyText.setString(ke::toString(mMoney));
	mMoneyText.setCharacterSize(20);
	mMoneyText.setFillColor(sf::Color::White);
	mMoneyText.setOutlineThickness(2.f);
	mMoneyText.setOutlineColor(sf::Color::Black);
	mMoneyText.setOrigin(0.f, mMoneyText.getGlobalBounds().height * 0.5f);
	mMoneyText.setPosition(50.f, 46.f - 4);
	mLevelText.setFont(font);
	mLevelText.setString("Level " + ke::toString(mLevel));
	mLevelText.setCharacterSize(40);
	mLevelText.setFillColor(sf::Color::White);
	mLevelText.setOutlineThickness(2.5f);
	mLevelText.setOutlineColor(sf::Color::Black);
	mLevelText.setOrigin(mLevelText.getGlobalBounds().width * 0.5f, 0.f);
	mLevelText.setPosition(mScene->getView().getSize().x * 0.5f, 10.f);

	// Create GUI for the end
	mWindow.setTexture(getApplication().getResource<ke::Texture>("gui-end"));
	mWindow.setPosition(320.f, 180.f);
	mTextResult.setFont(font);
	mTextResult.setCharacterSize(50);
	mTextResult.setString("Win !");
	mTextResult.setFillColor(sf::Color::White);
	mTextResult.setOutlineThickness(2.5f);
	mTextResult.setOutlineColor(sf::Color::Black);
	mTextResult.setOrigin(mTextResult.getGlobalBounds().width * 0.5f, 0.f);
	mTextResult.setPosition(640.f, 220.f);
	mTextCrystals.setFont(font);
	mTextCrystals.setCharacterSize(30);
	mTextCrystals.setString("+ 0");
	mTextCrystals.setFillColor(sf::Color::White);
	mTextCrystals.setOutlineThickness(2.f);
	mTextCrystals.setOutlineColor(sf::Color::Black);
	mTextCrystals.setPosition(320.f + 50.f, 350.f);
	mTextEnd.setFont(font);
	mTextEnd.setCharacterSize(30);
	mTextEnd.setString("Level " + ke::toString(mLevel + 1) + " unlocked !");
	mTextEnd.setFillColor(sf::Color::White);
	mTextEnd.setOutlineThickness(2.f);
	mTextEnd.setOutlineColor(sf::Color::Black);
	mTextEnd.setPosition(320.f + 50.f, 460.f);
	mTextNext.setFont(font);
	mTextNext.setCharacterSize(30);
	mTextNext.setString("Next");
	mTextNext.setFillColor(sf::Color::White);
	mTextNext.setOutlineThickness(2.f);
	mTextNext.setOutlineColor(sf::Color::Black);
	mTextNext.setPosition(800.f, 460.f);
}

GameState::~GameState()
{
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (!mEnded)
	{
		if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
		{
			sf::Vector2f p = getApplication().getWindow().getPointerPositionView(mScene->getView());
			bool handled = false;

			// Soldier button
			for (std::size_t i = 0; i < mSoldierButtons.size(); i++)
			{
				if (!handled && mSoldierButtons[i].getGlobalBounds().contains(p))
				{
					handled = true;
					getApplication().playSound("select");
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

			// Quit
			if (!handled && mReturnButton.getGlobalBounds().contains(p))
			{
				getApplication().playSound("select");
				endGame(0);
				handled = true;
			}

			// Settings
			if (!handled && mSettingsButton.getGlobalBounds().contains(p))
			{
				getApplication().playSound("select");
				pushState("SettingsState");
				handled = true;
			}

			// Map
			if (!handled && GameState::Bounds.contains(p) && mMoney >= mSoldierPrices[mSoldierSelected])
			{
				mMoney -= mSoldierPrices[mSoldierSelected];
				mScene->createActor<Pop>("", 2, mSoldierSelected)->setPosition(p);
				handled = true;
			}
		}
	}
	else if (mEnded && mEndGameTimer > sf::seconds(1.f))
	{
		if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
		{
			sf::Vector2f p = getApplication().getWindow().getPointerPositionView(mScene->getView());
			if (GameState::MenuButton.contains(p))
			{
				getApplication().playSound("select");
				delete mAI;
				delete mScene;
				clearStates();
				pushState("MenuState");
			}
			if (GameState::NextButton.contains(p))
			{
				getApplication().playSound("select");
				delete mAI;
				delete mScene;
				if (mResult == 2)
				{					
					newGame(mLevel + 1);
				}
				else
				{
					newGame(mLevel);
				}
			}
		}
	}
    return true;
}

bool GameState::update(sf::Time dt)
{
	if (!mEnded)
	{
		AI::LastHitPlayed += dt;

		// Clear AI Data
		AI::FramePosData.clear();

		// Remove dead actor
		for (std::size_t i = 0; i < mScene->getActorCount(); i++)
		{
			Entity::Ptr entity = mScene->getActorT<Entity>(i);
			if (entity != nullptr)
			{
				// Handle dead entities
				if (entity->isDead() && !entity->isHero())
				{
					if (entity->getTeam() == 1)
					{
						// Notify enemy death
						mAI->enemyDied();

						// Money Gain
						int gain = 40 + ke::random(0, 10);
						mMoney += gain;

						// Crystal Gain
						int crystalGain = 0;
						int rd = ke::random(0, 100);
						if (rd >= 80) // 15%
						{
							crystalGain = 1;
						}
						if (rd >= 95) // 5%
						{
							crystalGain = 1;
						}
						if (crystalGain >= 1)
						{
							getApplication().playSound("crystal");
						}
						mCrystalGained += crystalGain;

						// Add text
						mScene->createActor<GameText>("", gain, crystalGain)->setPosition(entity->getPosition());
					}

					mScene->createActor<Dead>("", entity->getTeam(), entity->getType())->setPosition(entity->getPosition());

					getApplication().playSound("die");

					// Remove
					entity->remove();
				}

				// AI Algorithm
				if (entity->isAlive())
				{
					AI::PosData d;
					d.id = entity->getId();
					d.pos = entity->getPosition();
					d.important = entity->isHero();
					d.team = entity->getTeam();
					d.box = entity->getBounds();
					AI::FramePosData.push_back(d);
				}
			}
		}

		// Money
		mMoneyTime += dt;
		if (mMoneyTime >= sf::seconds(1.f))
		{
			mMoneyTime = sf::Time::Zero;
			mMoney++;
		}
		mMoneyText.setString(ke::toString(mMoney));

		// AI
		mAI->update(dt);

		// Scene
		mScene->update(dt);

		AI::FramePosData.clear();

		// Hero Gui
		mHero->updateGui(dt);

		// End
		if (mHero->isDead())
		{
			endGame(1);
		}
		else if (mAI->hasLost())
		{
			endGame(2);
		}
	}
	else
	{
		mScene->update(dt);
		mEndGameTimer += dt;
	}
    return true;
}

void GameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
	mScene->render(target);

	if (!mEnded)
	{
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
			target.draw(mSoldierButtons[i]);
			target.draw(mSoldierSprites[i]);
		}

		mHero->renderGui(target);

		target.draw(mLevelText);
		target.draw(mReturnButton);
		target.draw(mSettingsButton);
		target.draw(mMoneyButton);
		target.draw(mMoneySprite);
		target.draw(mMoneyText);
	}

	if (mEnded)
	{
		// BLACK MASK
		sf::RectangleShape mask;
		float a = (mEndGameTimer < sf::seconds(1.f)) ? mEndGameTimer.asSeconds() * 128.f : 128.f;
		float c = (mEndGameTimer < sf::seconds(1.f)) ? 255.f - mEndGameTimer.asSeconds() * 236.f : 20.f;
		mask.setSize(sf::Vector2f(1280.f, 720.f));
		mask.setFillColor(sf::Color(static_cast<unsigned int>(c), static_cast<unsigned int>(c), static_cast<unsigned int>(c), static_cast<unsigned int>(a)));
		target.draw(mask);

		if (mEndGameTimer > sf::seconds(1.f))
		{
			target.draw(mWindow);
			target.draw(mTextResult);
			target.draw(mTextCrystals);
			target.draw(mTextEnd);
			target.draw(mTextNext);
		}
	}
}

void GameState::newGame(int level)
{
	mEnded = false;
	mCrystalGained = 0;
	mLevel = level;
	mResult = 0;

	GameState::Level = level;

	mLevelText.setString("Level " + ke::toString(mLevel));

	// Money
	mMoney = 300;
	if (mLevel == 0)
	{
		mMoney = 500;
	}
	else if (mLevel == 1)
	{
		mMoney = 400;
	}
	else
	{
		mMoney = 300;
	}

	// Log level start
	getApplication().getTime().setTimer(sf::seconds(0.001f), [this]()
	{
		ke::Log::log("Level started : " + ke::toString(mLevel));
	});

	// Create scene and AI
	mScene = new ke::Scene();
	mAI = new AIPlayer(*mScene, mLevel);

	// Update the view
	mScene->getView().setSize(sf::Vector2f(1280.f, 720.f));
	mScene->getView().setCenter(sf::Vector2f(640.f, 360.f));

	// Create the terrain and hero
	mScene->createActor<Terrain>("terrain");
	mHero = mScene->createActor<Hero>("hero");

	mEndGameTimer = sf::Time::Zero;
}

void GameState::endGame(int id)
{
	mEnded = true;
	AI::FramePosData.clear();
	mResult = id;

	if (GameState::FirstGame)
	{
		mCrystalGained++;
		GameState::FirstGame = false;
	}
	mCrystalGained++;
	
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	config.setProperty("game.crystals", mCrystalGained + config.getPropertyAs<int>("game.crystals"));
	config.setProperty("game.played", 1 + config.getPropertyAs<int>("game.played"));
	if (mResult == 2) // WIN
	{
		config.setProperty("game.level", mLevel + 1);
		mTextResult.setString("Win !");
		mTextResult.setOrigin(mTextResult.getGlobalBounds().width * 0.5f, 0.f);
		mTextEnd.setString("Level " + ke::toString(mLevel + 1) + " unlocked !");
		mTextNext.setString("Next");
	}
	else
	{
		mTextResult.setString("Loose");
		mTextResult.setOrigin(mTextResult.getGlobalBounds().width * 0.5f, 0.f);
		mTextEnd.setString("Carry on !");
		mTextNext.setString("Again");
	}
	mTextCrystals.setString("+ " + ke::toString(mCrystalGained));

	mCrystalGained = 0;
	mMoney = 0;
}
