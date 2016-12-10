#include "Soldier.hpp"

Soldier::Soldier(ke::Scene& scene, std::size_t soldierType)
	: AI(scene)
	, mSprite(nullptr)
	, mSoldierType(soldierType)
	, mPath(ke::random(50.f, 1024.f - 50.f), ke::random(50.f, 600.f)) // TODO : Set
{
	loadData();
}

Soldier::~Soldier()
{
}

void Soldier::initializeComponents()
{
	mSprite = createComponent<ke::AnimatorComponent>();
	attachComponent(mSprite);
	mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	mSprite->addAnimation("idle-east", "soldier-" + ke::toString(mSoldierType) + "-idle-east");
	mSprite->addAnimation("idle-north", "soldier-" + ke::toString(mSoldierType) + "-idle-north");
	mSprite->addAnimation("idle-west", "soldier-" + ke::toString(mSoldierType) + "-idle-west");
	mSprite->addAnimation("idle-south", "soldier-" + ke::toString(mSoldierType) + "-idle-south");
	mSprite->addAnimation("walk-east", "soldier-" + ke::toString(mSoldierType) + "-walk-east");
	mSprite->addAnimation("walk-north", "soldier-" + ke::toString(mSoldierType) + "-walk-north");
	mSprite->addAnimation("walk-west", "soldier-" + ke::toString(mSoldierType) + "-walk-west");
	mSprite->addAnimation("walk-south", "soldier-" + ke::toString(mSoldierType) + "-walk-south");
	mSprite->addAnimation("atk-east", "soldier-" + ke::toString(mSoldierType) + "-atk-east");
	mSprite->addAnimation("atk-north", "soldier-" + ke::toString(mSoldierType) + "-atk-north");
	mSprite->addAnimation("atk-west", "soldier-" + ke::toString(mSoldierType) + "-atk-west");
	mSprite->addAnimation("atk-south", "soldier-" + ke::toString(mSoldierType) + "-atk-south");
	mSprite->playAnimation("idle-south");
}

std::size_t Soldier::getTeam() const
{
	return 2;
}

std::size_t Soldier::getSoldierType() const
{
	return mSoldierType;
}

void Soldier::loadData()
{
	std::string type = "soldier-" + ke::toString(mSoldierType);
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	mLifeMax = config.getPropertyAs<int>(type + ".life");
	mLife = mLifeMax;
	mAttackCooldownMax = config.getPropertyAs<sf::Time>(type + ".cooldown");
	mDamage = config.getPropertyAs<int>(type + ".damage");
	mDistance = config.getPropertyAs<float>(type + ".distance");
	mSpeed = config.getPropertyAs<float>(type + ".speed");
}

void Soldier::onDirectionChanged()
{
	mSprite->playAnimation("walk-" + mDirection);
}

void Soldier::onStartMoving()
{
	mSprite->playAnimation("walk-" + mDirection);
}

void Soldier::onStopMoving()
{
	mSprite->playAnimation("idle-" + mDirection);
}

void Soldier::updateNoTarget(sf::Time dt)
{
	if (mTarget == nullptr)
	{
		if (ke::distance(mPath, getPosition()) < 50.f) // TODO : Set
		{
			mPath.x = ke::random(50.f, 1024.f - 50.f); // TODO : Set
			mPath.y = ke::random(50.f, 512.f); // TODO : Set
		}

		moveTo(mPath, dt);
	}
}
