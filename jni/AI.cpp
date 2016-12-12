#include "AI.hpp"
#include "Sources/Core/Scene.hpp"
#include "States/GameState.hpp"

std::vector<AI::PosData> AI::FramePosData;
sf::Time AI::LastHitPlayed;

AI::AI(ke::Scene& scene, std::size_t type)
	: Entity(scene, type)
	, mSprite(nullptr)
	, mDirection("so")
	, mMoving(false)
	, mPath(getPath())
	, mAttackCooldown(sf::Time::Zero)
	, mAttackCooldownMax(sf::seconds(1.f))
	, mAttackCast(sf::Time::Zero)
	, mTarget("")
	, mSpeed(100.f)
	, mDamage(20)
{
}

AI::~AI()
{
}

void AI::initializeComponents()
{
	Entity::initializeComponents();

	mSprite = createComponent<ke::AnimatorComponent>();
	attachComponent(mSprite);
	if (isGiant())
	{
		mSprite->setPosition(sf::Vector2f(-64.f, -108.f));
	}
	else
	{
		mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	}
	std::string type = "";
	if (getTeam() == 1)
	{
		type += "enemy-";
	}
	else if (getTeam() == 2)
	{
		type += "soldier-";
	}
	type += ke::toString(mType);
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
	mSprite->playAnimation("walk-so");
}

void AI::update(sf::Time dt)
{
	// ATTACKING ?
	if (mAttackCast > sf::Time::Zero) 
	{
		mAttackCast -= dt;
		if (mAttackCast <= sf::Time::Zero)
		{
			Entity::Ptr target = getScene().getActorT<Entity>(mTarget);
			if (target != nullptr)
			{
				target->inflige(mDamage);
				if (AI::LastHitPlayed > sf::seconds(0.1f))
				{
					getApplication().playSound("atk");
					AI::LastHitPlayed = sf::Time::Zero;
				}
			}
			if (mAttackCast < sf::Time::Zero)
			{
				mAttackCast = sf::Time::Zero;
			}
		}
	}
	else // NOT ATTACKING
	{
		// FIND TARGET
		int target = -1;
		float distance = 10000.f;
		for (std::size_t i = 0; i < AI::FramePosData.size(); i++)
		{
			PosData& d = AI::FramePosData[i];
			if (d.team != getTeam())
			{
				float dist = ke::distance(getPosition(), d.pos);
				if (d.important)
				{
					dist *= 0.8f;
				}
				if (target == -1 || dist < distance)
				{
					target = static_cast<int>(i);
					distance = dist;
				}
			}
		}

		// TARGET FOUND
		if (target != -1)
		{
			PosData& d = AI::FramePosData[target];

			// TARGET IS CLOSE ENOUGH TO ATTACK
			if (ke::distance(d.pos, getPosition()) < 50.f)
			{
				stopMoving();

				// Can attack ?
				if (mAttackCooldown <= sf::Time::Zero)
				{
					mSprite->playAnimation("atk-" + mDirection);
					mAttackCooldown = mAttackCooldownMax;
					mAttackCast = sf::seconds(0.5f);
					mTarget = d.id;
				}
			}
			else
			{
				moveTo(d.pos, dt, true);
			}
		}
		else // TARGET NOT FOUND
		{
			if (ke::distance(mPath, getPosition()) < 50.f)
			{
				mPath = getPath();
			}
			moveTo(mPath, dt, false);
		}
	}

	// COOLDOWN
	if (mAttackCooldown > sf::Time::Zero)
	{
		mAttackCooldown -= dt;
		if (mAttackCooldown < sf::Time::Zero)
		{
			mAttackCooldown = sf::Time::Zero;
		}
	}
}

bool AI::collide(const sf::Vector2f& mvt)
{
	sf::Vector2f pos = getPosition();
	pos.x += mvt.x;
	pos.y += mvt.y;
	for (std::size_t i = 0; i < AI::FramePosData.size(); i++)
	{
		PosData& d = AI::FramePosData[i];
		if (d.id != getId() && d.box.contains(pos))
		{
			return true;
		}
	}
	return false;
}

void AI::moveTo(const sf::Vector2f& dest, sf::Time dt, bool target)
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
		if (direction != mDirection) // Direction changed
		{
			mDirection = direction; 
			mSprite->playAnimation("walk-" + mDirection);
		}

		if (target)
		{
			if (!collide(mvt))
			{
				move(mvt);
				if (mvt != sf::Vector2f() && !mMoving)
				{
					mMoving = true;
					mSprite->playAnimation("walk-" + mDirection);
				}
			}
			else
			{
				// TRY ANOTHER DIRECTION
				ke::rotate(mvt, (ke::randomBool()) ? 90.f : -90.f);
				if (!collide(mvt))
				{
					move(mvt);
					if (mvt != sf::Vector2f() && !mMoving)
					{
						mMoving = true;
						mSprite->playAnimation("walk-" + mDirection);
					}
				}
				else
				{
					// THEN STOP
					stopMoving();
				}
			}

		}
		else // DOESNT HAVE TARGET
		{
			if (!collide(mvt))
			{
				move(mvt);
				if (mvt != sf::Vector2f() && !mMoving)
				{
					mMoving = true;
					mSprite->playAnimation("walk-" + mDirection);
				}
			}
			else
			{
				stopMoving();
			}
		}
	}
	else
	{
		stopMoving();
	}
}

void AI::stopMoving()
{
	if (mMoving)
	{
		mSprite->playAnimation("ilde-" + mDirection);
		mMoving = false;
		mPath = getPath();
	}
}

sf::Vector2f AI::getPath() const
{
	return sf::Vector2f(GameState::Bounds.left + ke::random(0.f, GameState::Bounds.width), GameState::Bounds.top + ke::random(0.f, GameState::Bounds.height));
}
