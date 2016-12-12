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

	mLifeMax = config.getPropertyAs<int>(type + ".life");
	mDamage = config.getPropertyAs<int>(type + ".damage");
	mSpeed = config.getPropertyAs<float>(type + ".speed");

	mLife = mLifeMax;
}