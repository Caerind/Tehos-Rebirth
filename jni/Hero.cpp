#include "Hero.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/Core/Scene.hpp"

Hero::Hero(ke::Scene& scene, std::size_t heroType)
	: Entity(scene)
	, mHero(nullptr)
	, mHeroType(heroType)
	, mCooldown(sf::Time::Zero)
	, mCooldownMax(sf::seconds(5.f))
{
	// TODO : Allow modification
	setPosition(sf::Vector2f(getScene().getView().getCenter().x, getScene().getView().getSize().y - 50.f));
	loadData();
}

Hero::~Hero()
{
}

void Hero::initializeComponents()
{
	mHero = createComponent<ke::AnimatorComponent>();
	attachComponent(mHero);
	mHero->addAnimation("idle", "hero-" + ke::toString(mHeroType) + "-idle");
	mHero->addAnimation("cast", "hero-" + ke::toString(mHeroType) + "-cast");
	mHero->playAnimation("idle");
	mHero->setPosition(sf::Vector2f(-16.f, -50.f)); // TODO : Set
}

sf::FloatRect Hero::getBounds() const
{
	float size = 16.f; // TODO : Set
	return sf::FloatRect(getPosition().x - size * 0.5f, getPosition().y - size * 0.5f, size, size);
}

void Hero::update(sf::Time dt)
{
	if (mCooldown > sf::Time::Zero)
	{
		mCooldown -= dt;
		if (mCooldown < sf::Time::Zero)
		{
			mCooldown = sf::Time::Zero;
		}
	}
}

std::size_t Hero::getTeam() const
{
	return 2;
}

std::size_t Hero::getHeroType() const
{
	return mHeroType;
}

bool Hero::canCast() const
{
	return (mCooldown <= sf::Time::Zero);
}

void Hero::cast()
{
	if (canCast())
	{
		// Play casting animation
		mHero->playAnimation("cast");

		// Cast 
		// TODO : Set
		getApplication().getTime().setTimer(sf::seconds(0.5f), [this]()
		{
			// TODO : Cast
			mCooldown = mCooldownMax;
		});

		// Return to idle animation
		// TODO : Set
		getApplication().getTime().setTimer(sf::seconds(0.8f), [this]()
		{
			mHero->playAnimation("idle");
		});
	}
}

float Hero::getCooldownPercent()
{
	return 100.f * (1.f - (mCooldown.asSeconds() / mCooldownMax.asSeconds()));
}

void Hero::loadData()
{
	std::string type = "hero-" + ke::toString(mHeroType);
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	mLifeMax = config.getPropertyAs<int>(type + ".life");
	mLife = mLifeMax;
	mCooldownMax = config.getPropertyAs<sf::Time>(type + ".cooldown");
}
