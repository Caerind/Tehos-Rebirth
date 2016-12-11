#include "AI.hpp"
#include "Sources/Core/Scene.hpp"

AI::AI(ke::Scene& scene)
	: Entity(scene)
	, mTarget(nullptr)
	, mBoxSize(8.f)
	, mDirection("so")
	, mAttackCooldown(sf::Time::Zero)
	, mAttackCooldownMax(sf::seconds(1.f))
	, mDistance(40.f)
	, mSpeed(100.f)
	, mDamage(20)
	, mTimerAttack(0)
{
}

AI::~AI()
{
	if (mTimerAttack != 0)
	{
		getApplication().getTime().stopTimer(mTimerAttack);
	}
}

void AI::update(sf::Time dt)
{
	findTarget();
	if (mTarget != nullptr)
	{
		updateTarget(dt);
	}
	else
	{
		updateNoTarget(dt);
	}

	if (mAttackCooldown > sf::Time::Zero)
	{
		mAttackCooldown -= dt;
		if (mAttackCooldown < sf::Time::Zero)
		{
			mAttackCooldown = sf::Time::Zero;
		}
	}
}

sf::FloatRect AI::getBounds() const
{
	sf::Vector2f p = getPosition();
	return sf::FloatRect(p.x - mBoxSize * 0.5f, p.y - mBoxSize * 0.5f, mBoxSize, mBoxSize);
}

void AI::attack()
{
	if (mTarget != nullptr && mTarget->isAlive() && ke::getLength(mTarget->getPosition() - getPosition()) < mDistance && mAttackCooldown <= sf::Time::Zero)
	{
		onStartAttack();
		mTimerAttack = getApplication().getTime().setTimer(sf::seconds(0.5f), [this]()
		{
			if (mTarget != nullptr && mTarget->isAlive())
			{
				onAttack();
			}
			mTimerAttack = 0;
		});
		mAttackCooldown = mAttackCooldownMax;
	}
}

void AI::updateNoTarget(sf::Time dt)
{
	if (mTarget == nullptr)
	{
		// Default : Nothing
	}
}

void AI::updateTarget(sf::Time dt)
{
	if (mTarget != nullptr)
	{
		if (ke::distance(mTarget->getPosition(), getPosition()) < mDistance)
		{
			stopMoving();

			attack();
		}
		else
		{
			moveTo(mTarget->getPosition(), dt);
		}
	}
}

void AI::findTarget()
{
	mTarget = nullptr;
	ke::Scene& scene = getScene();
	float distance = 10000.f;
	for (std::size_t i = 0; i < scene.getActorCount(); i++)
	{
		Entity::Ptr entity = scene.getActorT<Entity>(i);
		if (entity != nullptr && entity->isAlive() && entity->getTeam() != getTeam())
		{
			float d = ke::distance(getPosition(), entity->getPosition());
			if (mTarget == nullptr || d < distance)
			{
				mTarget = entity;
				distance = d;
			}
		}
	}
}

bool AI::collide(const sf::Vector2f& mvt)
{
	sf::FloatRect bounds = getBounds();
	bounds.left += mvt.x;
	bounds.top += mvt.y;
	ke::Scene& scene = getScene();
	for (std::size_t i = 0; i < scene.getActorCount(); i++)
	{
		Entity::Ptr entity = scene.getActorT<Entity>(i);
		if (entity != nullptr && entity->isAlive() && entity->getBounds().intersects(bounds) && entity->getId() != getId())
		{
			return true;
		}
	}
	return false;
}

void AI::moveTo(const sf::Vector2f& dest, sf::Time dt)
{
	sf::Vector2f delta = dest - getPosition();
	sf::Vector2f mvt = ke::normalized(delta) * mSpeed * dt.asSeconds();
	if (mvt != sf::Vector2f())
	{
		float angle = ke::getPolarAngle(mvt);
		std::string direction = "";
		if (angle >= 180.f && angle < 270.f)
		{
			direction = "no";
		}
		else if (angle >= 270.f && angle <= 360.f)
		{
			direction = "ne";
		}
		else if (angle >= 0 && angle < 90.f)
		{
			direction = "se";
		}
		else
		{
			direction = "so";
		}
		if (direction != mDirection)
		{
			mDirection = direction;
			onDirectionChanged();
		}
		if (!collide(mvt))
		{
			move(mvt);
			if (mvt != sf::Vector2f())
			{
				startMoving();
			}
		}
		else
		{
			stopMoving();
		}
	}
	else
	{
		stopMoving();
	}
}

void AI::startMoving()
{
	if (!mMoving)
	{
		mMoving = true;
		onStartMoving();
	}
}

void AI::stopMoving()
{
	if (mMoving)
	{
		onStopMoving();
		mMoving = false;
	}
}

void AI::onDirectionChanged()
{
}

void AI::onStartMoving()
{
}

void AI::onStopMoving()
{
}

void AI::onStartAttack()
{
}

void AI::onAttack()
{
	if (mTarget != nullptr)
	{
		mTarget->inflige(mDamage);
	}
}
