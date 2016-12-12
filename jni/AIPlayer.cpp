#include "AIPlayer.hpp"

sf::FloatRect AIPlayer::Area1 = sf::FloatRect(192, 128, 896, 512);
sf::FloatRect AIPlayer::Area2 = sf::FloatRect(192, 320, 192, 256);
sf::FloatRect AIPlayer::Area3 = sf::FloatRect(896, 320, 192, 256);

AIPlayer::AIPlayer(ke::Scene& scene, int level)
	: mScene(scene)
	, mLevel(level)
{

	mEnemiesLevel = getEnemiesLevel();
	mBaseTime = getBaseTime();


	mEnemiesToSpawn = mEnemiesLevel;
	mEnemiesCount = 0;
	mEnemiesDead = 0;

	mStarted = false;

	mElapsed = sf::Time::Zero;
	mDuration = sf::Time::Zero;
	mDelta = mBaseTime - getReduTime();
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::update(sf::Time dt)
{
	mElapsed += dt;
	if (!mStarted && (mLevel == 0 && mElapsed >= sf::seconds(5.f) || (mLevel > 0 && mElapsed >= sf::seconds(1.f))))
	{
		mStarted = true;
		mElapsed = sf::Time::Zero;
	}
	if (mStarted)
	{
		mDuration += dt;
		if (mElapsed >= mDelta && mEnemiesToSpawn > 0)
		{
			if (ke::random(1,4) >= 4 && mEnemiesToSpawn > 1)
			{
				spawnEnemy(0);
				spawnEnemy(0);
			}
			else
			{
				spawnEnemy(0);
			}
			mElapsed = sf::Time::Zero;
			mDelta = mBaseTime - getReduTime();
		}
	}
}

void AIPlayer::spawnEnemy(std::size_t type)
{
	sf::FloatRect area;
	switch (ke::random(1, 6))
	{
		case 1: area = Area2; break;
		case 2: area = Area3; break;
		default: area = Area1; break;
	}

	mScene.createActor<Pop>("", 1, type)->setPosition(sf::Vector2f(area.left + ke::random(0.f, area.width), area.top + ke::random(0.f, area.height)));

	mEnemiesToSpawn--;
	mEnemiesCount++;
}

void AIPlayer::enemyDied()
{
	mEnemiesCount--;
	mEnemiesDead++;
}

bool AIPlayer::hasLost() const
{
	return (mEnemiesCount <= 0 && mEnemiesDead >= mEnemiesLevel && mEnemiesToSpawn <= 0);
}

int AIPlayer::getEnemiesLevel() const
{
	return 10 * static_cast<int>(pow(1.1, mLevel)) + 2 * mLevel;
}

sf::Time AIPlayer::getBaseTime() const
{
	return sf::seconds((mLevel <= 0) ? 4.f : 2.f + 1.f / static_cast<float>(mLevel));
}

sf::Time AIPlayer::getReduTime() const
{
	return sf::seconds(std::min(1.f, 0.04f * mDuration.asSeconds()));
}
