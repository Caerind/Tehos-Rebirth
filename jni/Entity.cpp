#include "Entity.hpp"
#include "Sources/Core/Scene.hpp"
#include "Dead.hpp"

Entity::Entity(ke::Scene& scene)
	: ke::Actor(scene)
	, mShadow(nullptr)
	, mLife(1000)
	, mLifeMax(1000)
	, mTimerSlow(0)
	, mTimeFactor(1.f)
{
}

Entity::~Entity()
{
	if (mTimerSlow != 0)
	{
		getApplication().getTime().stopTimer(mTimerSlow);
	}
}

void Entity::initializeComponents()
{
	mShadow = createComponent<ke::SpriteComponent>();
	attachComponent(mShadow);
	mShadow->setTexture("fx");
	mShadow->setTextureRect(sf::IntRect(0, 0, 32, 32));
	mShadow->setPosition(sf::Vector2f(-16.f, -16.f));
	mShadow->setColor(sf::Color(1, 1, 1, 192));
	mShadow->setZ(-10.f);
}

void Entity::updateAll(sf::Time dt)
{
	dt *= mTimeFactor;
	updateComponents(dt);
	update(dt);
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

void Entity::onDie()
{
	getScene().createActor<Dead>("", getTeam(), 0)->setPosition(getPosition());
}

void Entity::slow(float factor, sf::Time slowDuration)
{
	mTimeFactor = factor;
	if (mTimerSlow == 0)
	{
		mTimerSlow = getApplication().getTime().setTimer(slowDuration, [this]()
		{
			mTimeFactor = 1.f;
			mTimerSlow = 0;
		});
	}
	else
	{
		getApplication().getTime().restartTimer(mTimerSlow, slowDuration);
	}
}
