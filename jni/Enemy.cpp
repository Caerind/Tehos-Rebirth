#include "Enemy.hpp"
#include "Sources/Core/Scene.hpp"
#include "Dead.hpp"

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
	Entity::initializeComponents();

	std::string type = "enemy-" + ke::toString(mEnemyType);
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

void Enemy::onDie()
{
	getScene().createActor<Dead>("", getTeam(), getEnemyType())->setPosition(getPosition());
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

void Enemy::onStartAttack()
{
	mSprite->playAnimation("atk-" + mDirection);
}
