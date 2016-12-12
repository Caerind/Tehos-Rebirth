#include "Entity.hpp"
#include "Sources/Core/Scene.hpp"
#include "Dead.hpp"

Entity::Entity(ke::Scene& scene, std::size_t type)
	: ke::Actor(scene)
	, mType(type)
	, mLife(100)
	, mLifeMax(100)
	, mTimeFactor(1.f)
	, mSlowRemaining(sf::Time::Zero)
{
	mLifeBar.setScale(sf::Vector2f(0.25f, 0.5f));
}

Entity::~Entity()
{
}

void Entity::initializeComponents()
{
	ke::SpriteComponent::Ptr shadow = createComponent<ke::SpriteComponent>();
	attachComponent(shadow);
	shadow->setTexture("fx");
	shadow->setTextureRect(sf::IntRect(0, 0, 32, 32));
	shadow->setPosition(sf::Vector2f(-16.f, -16.f));
	shadow->setColor(sf::Color(1, 1, 1, 192));
	if (isGiant() || isHero())
	{
		shadow->setPosition(sf::Vector2f(-32.f, -32.f));
		shadow->setScale(2.f, 2.f);
	}
	shadow->setZ(-10.f);
}

void Entity::updateAll(sf::Time dt)
{
	if (mSlowRemaining > sf::Time::Zero)
	{
		mSlowRemaining -= dt;
		if (mSlowRemaining < sf::Time::Zero)
		{
			mSlowRemaining = sf::Time::Zero;
			mTimeFactor = 0.f;
		}
		dt *= mTimeFactor;
	}
	updateComponents(dt);
	update(dt);
}

int Entity::getLife() const
{
	return mLife;
}

void Entity::inflige(int amount)
{
	mLife -= amount;
	if (mLife < 0)
	{
		mLife = 0;
	}
	if (mLife > mLifeMax)
	{
		mLife = mLifeMax;
	}
	mLifeBar.setPercent(getLifePercent());
}

void Entity::restore(int amount)
{
	mLife += amount;
	if (mLife < 0)
	{
		mLife = 0;
	}
	if (mLife > mLifeMax)
	{
		mLife = mLifeMax;
	}
	mLifeBar.setPercent(getLifePercent());
}

bool Entity::isDead() const
{
	return (mLife <= 0);
}

bool Entity::isAlive() const
{
	return (mLife > 0);
}

float Entity::getLifePercent() const
{
	return (static_cast<float>(mLife)) / (static_cast<float>(mLifeMax));
}

sf::FloatRect Entity::getBounds() const
{
	sf::Vector2f p = getPosition();

	if (isGiant() || isHero())
	{
		return sf::FloatRect(p.x - 4.f, p.y - 4.f, 8.f, 8.f);
	}
	return sf::FloatRect(p.x - 2.f, p.y - 2.f, 4.f, 4.f);
}

void Entity::render(sf::RenderTarget& target)
{
	#ifdef _DEBUG
	sf::FloatRect bounds = getBounds();
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(bounds.width, bounds.height));
	shape.setPosition(bounds.left, bounds.top);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Red);
	target.draw(shape);
	#endif

	if (isGiant())
	{
		mLifeBar.setPosition(getPosition() + sf::Vector2f(-37.5f, -130.f));
	}
	else
	{
		mLifeBar.setPosition(getPosition() + sf::Vector2f(-37.5f, -65.f));
	}
	mLifeBar.render(target);
}

std::size_t Entity::getTeam() const
{
	return 0;
}

std::size_t Entity::getType() const
{
	return mType;
}

void Entity::slow(float factor, sf::Time slowDuration)
{
	mSlowRemaining = slowDuration;
	mTimeFactor = factor;
}

bool Entity::isHero() const
{
	return false;
}

bool Entity::isGiant() const
{
	return false;
}
