#include "Entity.hpp"

Entity::Entity(ke::Scene& scene)
	: ke::Actor(scene)
	, mLife(1000)
	, mLifeMax(1000)
{
}

Entity::~Entity()
{
}

void Entity::serialize(ke::Serializer& serializer)
{
}

bool Entity::deserialize(ke::Serializer& serializer)
{
	return false;
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
	return sf::FloatRect();
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
}

std::size_t Entity::getTeam() const
{
	return 0;
}
