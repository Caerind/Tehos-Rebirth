#include "Soldier.hpp"
#include "Sources/Core/Scene.hpp"
#include "Dead.hpp"

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
	Entity::initializeComponents();

	std::string type = "soldier-" + ke::toString(mSoldierType);
	mSprite = createComponent<ke::AnimatorComponent>();
	attachComponent(mSprite);
	mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	mSprite->addAnimation("idle-so", type + "-idle-so");
	mSprite->addAnimation("idle-se", type + "-idle-se");
	mSprite->addAnimation("idle-ne", type + "-idle-ne");
	mSprite->addAnimation("idle-no", type + "-idle-no");
	mSprite->addAnimation("walk-so", type + "-walk-so");
	mSprite->addAnimation("walk-se", type + "-walk-se");
	mSprite->addAnimation("walk-ne", type + "-walk-ne");
	mSprite->addAnimation("walk-no", type + "-walk-no");
	mSprite->addAnimation("atk-so", type + "-atk-so");
	mSprite->addAnimation("atk-se", type + "-atk-se");
	mSprite->addAnimation("atk-ne", type + "-atk-ne");
	mSprite->addAnimation("atk-no", type + "-atk-no");
	mSprite->playAnimation("idle-so");
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

void Soldier::onDie()
{
	getScene().createActor<Dead>("", getTeam(), getSoldierType())->setPosition(getPosition());
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

void Soldier::onStartAttack()
{
	mSprite->playAnimation("atk-" + mDirection);
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
