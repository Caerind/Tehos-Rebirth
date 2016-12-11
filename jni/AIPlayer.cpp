#include "AIPlayer.hpp"

AIPlayer::AIPlayer(ke::Scene& scene, int level)
	: mScene(scene)
	, mLevel(level)
{

	mEnemiesLevel = 10 * static_cast<int>(pow(1.1, level)) + 2 * level;
	mBaseTime = sf::seconds((mLevel <= 0) ? 4.f : 2.f + 1.f / static_cast<float>(mLevel));




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
			spawnEnemy(0);
			mElapsed = sf::Time::Zero;
			mDelta = mBaseTime - getReduTime();
		}
	}

	ke::Application::getWindow().setDebugInfo("enemiescount", ke::toString(mEnemiesCount));
	ke::Application::getWindow().setDebugInfo("enemiesdead", ke::toString(mEnemiesDead));
	ke::Application::getWindow().setDebugInfo("enemiestospawn", ke::toString(mEnemiesToSpawn));
	ke::Application::getWindow().setDebugInfo("enemieslevel", ke::toString(mEnemiesLevel));
}

void AIPlayer::spawnEnemy(std::size_t type)
{
	sf::FloatRect area = sf::FloatRect(140, 80, 760, 260);
	int r = ke::random(1, 5);
	if (r == 1)
	{
		area = sf::FloatRect(140, 340, 100, 340);
	}
	else if (r == 2)
	{
		area = sf::FloatRect(800, 340, 100, 340);
	}

	sf::Vector2f pos = sf::Vector2f(area.left + ke::random(0.f, area.width), area.top + ke::random(0.f, area.height));
	
	mScene.createActor<Pop>("", 1, type)->setPosition(pos);

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
	return (mEnemiesCount == 0 && mEnemiesDead == mEnemiesLevel && mEnemiesToSpawn == 0);
}

int AIPlayer::getEnemiesLevel() const
{
	return mEnemiesLevel;
}

int AIPlayer::getEnemiesCount() const
{
	return mEnemiesCount;
}

int AIPlayer::getEnemiesToSpawn() const
{
	return mEnemiesToSpawn;
}

int AIPlayer::getEnemiesDead() const
{
	return mEnemiesDead;
}

sf::Time AIPlayer::getReduTime() const
{
	return sf::seconds(std::min(1.f, 0.02f * mDuration.asSeconds()));
}
