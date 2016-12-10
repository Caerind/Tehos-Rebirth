#include "Enemy.hpp"

Enemy::Enemy(ke::Scene& scene, std::size_t enemyType)
	: AI(scene)
	, mSprite(nullptr)
	, mEnemyType(enemyType)
{
	loadData();
}

Enemy::~Enemy()
{
}

void Enemy::initializeComponents()
{
	std::string type = "enemy-" + ke::toString(mEnemyType);
	mSprite = createComponent<ke::AnimatorComponent>();
	attachComponent(mSprite);
	mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	mSprite->addAnimation("idle-east", type + "-idle-east");
	mSprite->addAnimation("idle-north", type + "-idle-north");
	mSprite->addAnimation("idle-west", type + "-idle-west");
	mSprite->addAnimation("idle-south", type + "-idle-south");
	mSprite->addAnimation("walk-east", type + "-walk-east");
	mSprite->addAnimation("walk-north", type + "-walk-north");
	mSprite->addAnimation("walk-west", type + "-walk-west");
	mSprite->addAnimation("walk-south", type + "-walk-south");
	mSprite->addAnimation("atk-east", type + "-atk-east");
	mSprite->addAnimation("atk-north", type + "-atk-north");
	mSprite->addAnimation("atk-west", type + "-atk-west");
	mSprite->addAnimation("atk-south", type + "-atk-south");
	mSprite->playAnimation("idle-south");
}

std::size_t Enemy::getTeam() const
{
	return 1;
}

std::size_t Enemy::getEnemyType() const
{
	return mEnemyType;
}

void Enemy::loadData()
{
	std::string type = "enemy-" + ke::toString(mEnemyType);
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	mLifeMax = config.getPropertyAs<int>(type + ".life");
	mLife = mLifeMax;
	mAttackCooldownMax = config.getPropertyAs<sf::Time>(type + ".cooldown");
	mDamage = config.getPropertyAs<int>(type + ".damage");
	mDistance = config.getPropertyAs<float>(type + ".distance");
	mSpeed = config.getPropertyAs<float>(type + ".speed");
}

void Enemy::onDirectionChanged()
{
	mSprite->playAnimation("walk-" + mDirection);
}

void Enemy::onStartMoving()
{
	mSprite->playAnimation("walk-" + mDirection);
}

void Enemy::onStopMoving()
{
	mSprite->playAnimation("ilde-" + mDirection);
}
