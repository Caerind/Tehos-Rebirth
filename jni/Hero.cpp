#include "Hero.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/Core/Scene.hpp"
#include "States/GameState.hpp"

Hero::Hero(ke::Scene& scene)
	: Entity(scene)
	, mHero(nullptr)
	, mCooldown(sf::Time::Zero)
	, mCooldownMax(sf::seconds(5.f))
{
	setPosition(sf::Vector2f(getScene().getView().getCenter().x, getScene().getView().getSize().y - 80.f));

	loadData();

	mHeroSpell1.setTexture(getApplication().getResource<ke::Texture>("gui-game"));
	mHeroSpell1.setTextureRect(sf::IntRect(460 + (mSpell1 * 92), 0, 92, 92));
	mHeroSpell1.setPosition(getScene().getView().getSize() - sf::Vector2f(184.f, 92.f));
	mHeroSpell2.setTexture(getApplication().getResource<ke::Texture>("gui-game"));
	mHeroSpell2.setTextureRect(sf::IntRect(460 + (mSpell2 * 92), 0, 92, 92));
	mHeroSpell2.setPosition(getScene().getView().getSize() - sf::Vector2f(92.f, 92.f));
	mHeroCooldown1.setTexture(getApplication().getResource<ke::Texture>("gui-game"));
	mHeroCooldown1.setTextureRect(sf::IntRect(460 + (mSpell1 * 92), 92, 92, 92));
	mHeroCooldown1.setPosition(getScene().getView().getSize() - sf::Vector2f(184.f, 92.f));
	mHeroCooldown1.setColor(sf::Color(20, 20, 20, 128));
	mHeroCooldown2.setTexture(getApplication().getResource<ke::Texture>("gui-game"));
	mHeroCooldown2.setTextureRect(sf::IntRect(460 + (mSpell2 * 92), 92, 92, 92));
	mHeroCooldown2.setPosition(getScene().getView().getSize() - sf::Vector2f(92.f, 92.f));
	mHeroCooldown2.setColor(sf::Color(20, 20, 20, 128));

	mLifeBar.setPosition(sf::Vector2f(getScene().getView().getSize().x * 0.5f - 300.f, getScene().getView().getSize().y));
	mLifeBar.setScale(sf::Vector2f(1.f, -1.f));
	mLifeBar.setPercent(99.99f); // Display only
}

Hero::~Hero()
{
}

void Hero::initializeComponents()
{
	Entity::initializeComponents();

	mHero = createComponent<ke::AnimatorComponent>();
	attachComponent(mHero);
	mHero->addAnimation("idle", "hero-idle");
	mHero->addAnimation("cast", "hero-cast");
	mHero->playAnimation("idle");
	mHero->setPosition(sf::Vector2f(-64.f, -108.f));
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

bool Hero::handleGui(const sf::Vector2f& pointer)
{
	bool handled = false;
	if (mHeroSpell1.getGlobalBounds().contains(pointer))
	{
		cast(1);
		handled = true;
	}
	else if (mHeroSpell2.getGlobalBounds().contains(pointer))
	{
		cast(2);
		handled = true;
	}
	return handled;
}

void Hero::updateGui(sf::Time dt)
{
	if (!canCast())
	{
		float cooldownSize = getCooldownPercent() * 0.92f;
		mHeroCooldown1.setTextureRect(sf::IntRect(460 + (mSpell1 * 92), 92 + static_cast<int>(cooldownSize), 92, 92 - static_cast<int>(cooldownSize)));
		mHeroCooldown1.setPosition(getScene().getView().getSize() - sf::Vector2f(184.f, 92.f - cooldownSize));
		mHeroCooldown2.setTextureRect(sf::IntRect(460 + (mSpell2 * 92), 92 + static_cast<int>(cooldownSize), 92, 92 - static_cast<int>(cooldownSize)));
		mHeroCooldown2.setPosition(getScene().getView().getSize() - sf::Vector2f(92.f, 92.f - cooldownSize));
	}
}

void Hero::renderGui(sf::RenderTarget& target)
{
	target.draw(mHeroSpell1);
	target.draw(mHeroSpell2);
	if (!canCast())
	{
		target.draw(mHeroCooldown1);
		target.draw(mHeroCooldown2);
	}
	mLifeBar.render(target);
}

void Hero::render(sf::RenderTarget& target)
{
}

bool Hero::canCast() const
{
	return (mCooldown <= sf::Time::Zero);
}

void Hero::cast(std::size_t buttonIndex)
{
	if (canCast() && ((buttonIndex == 1 && mSpell1 > 0) || (buttonIndex == 2 && mSpell2 > 0)))
	{
		// Play casting animation
		mHero->playAnimation("cast");
		mCooldown = mCooldownMax;

		// Cast 
		getApplication().getTime().setTimer(sf::seconds(0.3f), [buttonIndex, this]()
		{
			if (buttonIndex == 1)
			{
				castSpell(mSpell1);
			}
			else if (buttonIndex == 2)
			{
				castSpell(mSpell2);
			}
		});

		// Return to idle animation
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
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	int bonusLife = static_cast<int>(2 * std::pow(1.2, config.getPropertyAs<int>("hero.slife")));
	mLifeMax = config.getPropertyAs<int>("hero.life") + bonusLife;
	mSpell1 = config.getPropertyAs<int>("hero.spell-1");
	mSpell2 = config.getPropertyAs<int>("hero.spell-2");
	mLife = mLifeMax;
	mHealPower = 1;
	mDamagePower = 1;
	mSlowPower = sf::Time::Zero;
	mShockPower = 1.f;
	if (mSpell1 == 1 || mSpell2 == 1)
	{
		int bonusHeal = static_cast<int>(2 * std::pow(1.2, config.getPropertyAs<int>("hero.s1")));
		mHealPower = 50 + bonusHeal;
	}
	if (mSpell1 == 2 || mSpell2 == 2)
	{
		int bonusDamage = static_cast<int>(2 * std::pow(1.2, config.getPropertyAs<int>("hero.s2")));
		mDamagePower = 40 + bonusDamage;
	}
	if (mSpell1 == 3 || mSpell2 == 3)
	{
		float bonusSlow = static_cast<float>(2 * std::pow(1.2, config.getPropertyAs<int>("hero.s3")));
		mSlowPower = sf::seconds(3.f) + sf::seconds(bonusSlow);
	}
	if (mSpell1 == 4 || mSpell2 == 4)
	{
		float bonusShock = static_cast<float>(2 * std::pow(1.2, config.getPropertyAs<int>("hero.s4")));
		mShockPower = 90.f + bonusShock;
	}
}

void Hero::castSpell(int spellId)
{
	ke::Scene& scene = getScene();
	if (spellId == 1) // Heal
	{
		getApplication().playSound("sp-1");
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() == getTeam())
			{
				entity->restore(mHealPower);
				getScene().createActor<GameEffect>("", 1)->setPosition(entity->getPosition());
			}
		}
	}
	else if (spellId == 2) // Damage
	{
		getApplication().playSound("sp-2");
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() != getTeam())
			{
				entity->inflige(mDamagePower);
				getScene().createActor<GameEffect>("", 2)->setPosition(entity->getPosition());
			}
		}
	}
	else if (spellId == 3) // Slow
	{
		getApplication().playSound("sp-3");
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() != getTeam())
			{
				entity->slow(0.5f, mSlowPower);
			}
		}
	}
	else if (spellId == 4) // Shock
	{
		getApplication().playSound("sp-4");
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() != getTeam())
			{
				sf::Vector2f mvt = ke::normalized(entity->getPosition() - getPosition()) * mShockPower;
				if (mvt != sf::Vector2f())
				{
					GameEffect::Ptr effect = getScene().createActor<GameEffect>("", 4);
					effect->setPosition(entity->getPosition());
					effect->setRotation(ke::getPolarAngle(mvt) + 90.f);
				}
				entity->move(mvt);
				if (!GameState::Bounds.contains(entity->getPosition()))
				{
					entity->inflige(100000); // Kill
				}
			}
		}
	}
}

bool Hero::isHero() const
{
	return true;
}
