#include "Pop.hpp"
#include "Soldier.hpp"
#include "Enemy.hpp"
#include "Sources\Core\Scene.hpp"

Pop::Pop(ke::Scene& scene, std::size_t team, std::size_t type)
	: ke::Actor(scene)
	, mSprite(nullptr)
	, mFx(nullptr)
	, mParticles(nullptr)
	, mTeam(team)
	, mType(type)
	, mElapsed(sf::Time::Zero)
{
}

Pop::~Pop()
{
}

void Pop::initializeComponents()
{
	mSprite = createComponent<ke::AnimatorComponent>();
	std::string type = "";
	switch (mTeam)
	{
		case 1: type += "enemy-" + ke::toString(mType); break;
		case 2: type += "soldier-" + ke::toString(mType); break;
		default: break;
	}
	attachComponent(mSprite);
	mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	mSprite->addAnimation("pop", type + "-pop");
	mSprite->playAnimation("pop");

	if (mTeam == 1)
	{
		mFx = createComponent<ke::SpriteComponent>();
		attachComponent(mFx);
		mFx->setTexture("fx");
		mFx->setTextureRect(sf::IntRect(64, 0, 32, 32));
		mFx->setPosition(sf::Vector2f(-16.f, -16.f));
		mFx->setColor(sf::Color(150, 150, 150, 0));

		mParticles = createComponent<ke::ParticleComponent>();
		attachComponent(mParticles);
		mParticles->setTexture("fx");
		mParticles->addTextureRect(sf::IntRect(32 + 14, 14, 4, 4));
		mParticles->setParticleVelocity(ke::Distributions::project(sf::Vector2f(0.f, -1.f), 35.f, 60.f, 150.f));
		mParticles->setParticleScale(ke::Distributions::rect(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.5f, 0.5f)));
		mParticles->setParticleLifetime(ke::Distributions::uniform(sf::seconds(0.2f), sf::seconds(0.4f)));
		mParticles->setParticleRotationSpeed(ke::Distributions::uniform(45.f, 90.f));
		mParticles->setParticleColor(ke::Distributions::colorGrade(sf::Color(120, 70, 20), 0.6f, 1.f));
		mParticles->emitParticles(50);
	}
	if (mTeam == 2)
	{
		mFx = createComponent<ke::SpriteComponent>();
		attachComponent(mFx);
		mFx->setTexture("fx");
		mFx->setTextureRect(sf::IntRect(0, 0, 32, 32));
		mFx->setPosition(sf::Vector2f(-16.f, -16.f));
		mFx->setColor(sf::Color(1, 1, 1, 192));
		mFx->setZ(-10.f);

		mParticles = createComponent<ke::ParticleComponent>();
		attachComponent(mParticles);
		mParticles->setTexture("fx");
		mParticles->addTextureRect(sf::IntRect(32 + 14, 14, 4, 4));
		mParticles->setParticleVelocity(ke::Distributions::project(sf::Vector2f(0.f, 1.f), 360.f, 75.f, 150.f));
		mParticles->setParticleScale(ke::Distributions::rect(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.25f, 0.25f)));
		mParticles->setParticleLifetime(ke::Distributions::uniform(sf::seconds(0.2f), sf::seconds(0.4f)));
		mParticles->setParticleRotationSpeed(ke::Distributions::uniform(0.f, 180.f));
		mParticles->setParticleColor(ke::Distributions::colorGrade(sf::Color(250, 250, 30), 0.5f, 1.f));
		mParticles->emitParticles(50);
		mParticles->setZ(-100.f);
	}
}

void Pop::update(sf::Time dt)
{
	mElapsed += dt;
	if (mElapsed < sf::seconds(0.1f))
	{
		float a = mElapsed.asSeconds() * 1590.f + 96.f;
		mFx->setColor(sf::Color(150, 150, 150, static_cast<unsigned int>(a)));
	}
	else
	{
		float a = (1.1f - mElapsed.asSeconds()) * 159.f + 96.f;
		mFx->setColor(sf::Color(150, 150, 150, static_cast<unsigned int>(a)));
	}
	if (mElapsed > sf::seconds(1.f))
	{
		remove();
		if (mTeam == 1)
		{
			getScene().createActor<Enemy>("", mType)->setPosition(getPosition());
		}
		else if (mTeam == 2)
		{
			getScene().createActor<Soldier>("", mType)->setPosition(getPosition());
		}
	}
}
