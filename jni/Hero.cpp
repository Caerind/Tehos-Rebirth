#include "Hero.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/Core/Scene.hpp"
#include "States/GameState.hpp"

Hero::Hero(ke::Scene& scene, std::size_t heroType)
	: Entity(scene)
	, mHero(nullptr)
	, mHeroType(heroType)
	, mCooldown(sf::Time::Zero)
	, mCooldownMax(sf::seconds(5.f))
	, mHeroSpell1("gui-game")
	, mHeroSpell2("gui-game")
{
	// TODO : Allow modification
	setPosition(sf::Vector2f(getScene().getView().getCenter().x, getScene().getView().getSize().y - 80.f));

	loadData();

	mHeroSpell1.setTextureRect(sf::IntRect(460 + (mSpell1 * 92), 0, 92, 92));
	mHeroSpell1.setPosition(getScene().getView().getSize() - sf::Vector2f(184.f, 92.f));
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

	mLifeBar.setPosition(sf::Vector2f(getScene().getView().getSize().x * 0.5f - 300.f, getScene().getView().getSize().y - 80.f));
	mLifeBar.setScale(sf::Vector2f(2.f, 2.f));
}

Hero::~Hero()
{
}

void Hero::initializeComponents()
{
	mHero = createComponent<ke::AnimatorComponent>();
	attachComponent(mHero);
	mHero->addAnimation("idle", "hero-idle");
	mHero->addAnimation("cast", "hero-cast");
	mHero->playAnimation("idle");
	mHero->setPosition(sf::Vector2f(-64.f, -108.f)); // TODO : Set
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

bool Hero::handleGui(const sf::Vector2f& pointer)
{
	bool handled = false;
	if (mHeroSpell1.getBounds().contains(pointer))
	{
		cast(1);
		handled = true;
	}
	else if (mHeroSpell2.getBounds().contains(pointer))
	{
		cast(2);
		handled = true;
	}
	return handled;
}

void Hero::updateGui(sf::Time dt)
{
	float cooldownSize = getCooldownPercent() * 0.92f;
	mHeroCooldown1.setTextureRect(sf::IntRect(460 + (mSpell1 * 92), 92 + static_cast<int>(cooldownSize), 92, 92 - static_cast<int>(cooldownSize)));
	mHeroCooldown1.setPosition(getScene().getView().getSize() - sf::Vector2f(184.f, 92.f - cooldownSize));
	mHeroCooldown2.setTextureRect(sf::IntRect(460 + (mSpell2 * 92), 92 + static_cast<int>(cooldownSize), 92, 92 - static_cast<int>(cooldownSize)));
	mHeroCooldown2.setPosition(getScene().getView().getSize() - sf::Vector2f(92.f, 92.f - cooldownSize));
	mLifeBar.setPercent(getLifePercent());
}

void Hero::renderGui(sf::RenderTarget& target)
{
	mHeroSpell1.render(target);
	mHeroSpell2.render(target);
	target.draw(mHeroCooldown1);
	target.draw(mHeroCooldown2);
	mLifeBar.render(target);
}

bool Hero::canCast() const
{
	return (mCooldown <= sf::Time::Zero);
}

void Hero::cast(std::size_t buttonIndex)
{
	if (canCast())
	{
		// Play casting animation
		mHero->playAnimation("cast");
		mCooldown = mCooldownMax;

		// Cast 
		// TODO : Set
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
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	mLifeMax = config.getPropertyAs<int>("hero.life");
	mCooldownMax = config.getPropertyAs<sf::Time>("hero.cooldown");
	mSpell1 = config.getPropertyAs<int>("hero.spell-1");
	mSpell2 = config.getPropertyAs<int>("hero.spell-2");
	mLife = mLifeMax;
	if (mSpell1 == 0 && mSpell2 == 0)
	{
		mSpell1 = 1;
		mSpell2 = 2;
	}
}

void Hero::castSpell(int spellId)
{
	ke::Scene& scene = getScene();
	if (spellId == 1) // Heal
	{
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() == getTeam())
			{
				entity->restore(50); // TODO : Set
				getScene().createActor<GameEffect>("", 1)->setPosition(entity->getPosition());
			}
		}
	}
	else if (spellId == 2) // Damage
	{
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() != getTeam())
			{
				entity->inflige(50); // TODO : Set
				getScene().createActor<GameEffect>("", 2)->setPosition(entity->getPosition());
			}
		}
	}
	else if (spellId == 3) // Slow
	{
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() != getTeam())
			{
				entity->slow(0.5f, sf::seconds(2.f)); // TODO : Set
			}
		}
	}
	else if (spellId == 4) // Shock
	{
		for (std::size_t i = 0; i < scene.getActorCount(); i++)
		{
			Entity::Ptr entity = scene.getActorT<Entity>(i);
			if (entity != nullptr && entity->isAlive() && entity->getTeam() != getTeam())
			{
				sf::Vector2f mvt = ke::normalized(entity->getPosition() - getPosition()) * 60.f; // TODO : Set
				if (mvt != sf::Vector2f())
				{
					GameEffect::Ptr effect = getScene().createActor<GameEffect>("", 3);
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

void Hero::render(sf::RenderTarget& target)
{
}
