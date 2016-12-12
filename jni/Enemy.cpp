#include "Enemy.hpp"
#include "Sources/Core/Scene.hpp"
#include "Dead.hpp"

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
	return (mType == 1);
}

void Enemy::loadData()
{
	std::string type = "enemy-" + ke::toString(mType);
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");

	mLifeMax = config.getPropertyAs<int>(type + ".life");
	mDamage = config.getPropertyAs<int>(type + ".damage");
	mSpeed = config.getPropertyAs<float>(type + ".speed");

	mLife = mLifeMax;
}