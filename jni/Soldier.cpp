#include "Soldier.hpp"
#include "Sources/Core/Scene.hpp"
#include "Dead.hpp"
#include "States\GameState.hpp"

Soldier::Soldier(ke::Scene& scene, std::size_t type)
	: AI(scene, type)
{
	loadData();
}

Soldier::~Soldier()
{
}

std::size_t Soldier::getTeam() const
{
	return 2;
}

bool Soldier::isGiant() const
{
	return false;
}

void Soldier::loadData()
{
	std::string type = "soldier-" + ke::toString(mType);
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	int bonusLife = static_cast<int>(2 * std::pow(1.2, config.getPropertyAs<int>(type + ".slife")));
	int bonusDamage = static_cast<int>(2 * std::pow(1.2, config.getPropertyAs<int>(type + ".sdamage")));
	mLifeMax = config.getPropertyAs<int>(type + ".life") + bonusLife;
	mDamage = config.getPropertyAs<int>(type + ".damage") + bonusDamage;
	mSpeed = config.getPropertyAs<float>(type + ".speed");

	mLife = mLifeMax;
}