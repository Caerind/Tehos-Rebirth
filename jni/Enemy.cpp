#include "Enemy.hpp"
#include "Sources/Core/Scene.hpp"
#include "Dead.hpp"
#include "States/GameState.hpp"

Enemy::Enemy(ke::Scene& scene, std::size_t type)
	: AI(scene, type)
{
	loadData();
}

Enemy::~Enemy()
{
}

std::size_t Enemy::getTeam() const
{
	return 1;
}

bool Enemy::isGiant() const
{
	return (mType == 2);
}

void Enemy::loadData()
{
	std::string type = "enemy-" + ke::toString(mType);
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");

	int bonusWave = static_cast<int>(2 * std::pow(1.2, GameState::Level)) - 2;
	mLifeMax = config.getPropertyAs<int>(type + ".life") + bonusWave;
	mDamage = config.getPropertyAs<int>(type + ".damage") + bonusWave;
	mSpeed = config.getPropertyAs<float>(type + ".speed");

	mLife = mLifeMax;
}