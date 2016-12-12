#include "Pop.hpp"
#include "Soldier.hpp"
#include "Enemy.hpp"
#include "Sources\Core\Scene.hpp"

Pop::Pop(ke::Scene& scene, std::size_t team, std::size_t type)
	: ke::Actor(scene)
	, mFx(nullptr)
	, mTeam(team)
	, mType(type)
	, mElapsed(sf::Time::Zero)
{
	if (mTeam == 1)
	{
		getApplication().playSound("spawn-e");
	}
	else if (mTeam == 2)
	{
		getApplication().playSound("spawn-s");
	}
}

Pop::~Pop()
{
}

void Pop::initializeComponents()
{
	ke::AnimatorComponent::Ptr sprite = createComponent<ke::AnimatorComponent>();
	std::string type = "";
	switch (mTeam)
	{
		case 1: type += "enemy-" + ke::toString(mType); break;
		case 2: type += "soldier-" + ke::toString(mType); break;
		default: break;
	}
	attachComponent(sprite);
	if (mTeam == 1 && mType == 1)
	{
		sprite->setPosition(sf::Vector2f(-64.f, -108.f));
	}
	else
	{
		sprite->setPosition(sf::Vector2f(-32.f, -54.f));
	}
	sprite->addAnimation("pop", type + "-pop");
	sprite->playAnimation("pop");

	if (mTeam == 1)
	{
		mFx = createComponent<ke::SpriteComponent>();
		attachComponent(mFx);
		mFx->setTexture("fx");
		mFx->setTextureRect(sf::IntRect(64, 0, 32, 32));
		mFx->setPosition(sf::Vector2f(-16.f, -16.f));
		mFx->setColor(sf::Color(150, 150, 150, 0));

		ke::ParticleComponent::Ptr particles = createComponent<ke::ParticleComponent>();
		attachComponent(particles);
		particles->setTexture("fx");
		particles->addTextureRect(sf::IntRect(32 + 14, 14, 4, 4));
		particles->setParticleVelocity(ke::Distributions::project(sf::Vector2f(0.f, -1.f), 35.f, 60.f, 150.f));
		particles->setParticleScale(ke::Distributions::rect(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.5f, 0.5f)));
		particles->setParticleLifetime(ke::Distributions::uniform(sf::seconds(0.1f), sf::seconds(0.35f)));
		particles->setParticleRotationSpeed(ke::Distributions::uniform(45.f, 90.f));
		particles->setParticleColor(ke::Distributions::colorGrade(sf::Color(120, 70, 20), 0.6f, 1.f));
		particles->emitParticles(50);
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

		ke::ParticleComponent::Ptr particles = createComponent<ke::ParticleComponent>();
		attachComponent(particles);
		particles->setTexture("fx");
		particles->addTextureRect(sf::IntRect(32 + 14, 14, 4, 4));
		particles->setParticleVelocity(ke::Distributions::project(sf::Vector2f(0.f, 1.f), 360.f, 75.f, 150.f));
		particles->setParticleScale(ke::Distributions::rect(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.25f, 0.25f)));
		particles->setParticleLifetime(ke::Distributions::uniform(sf::seconds(0.1f), sf::seconds(0.35f)));
		particles->setParticleRotationSpeed(ke::Distributions::uniform(0.f, 180.f));
		particles->setParticleColor(ke::Distributions::colorGrade(sf::Color(250, 250, 30), 0.5f, 1.f));
		particles->emitParticles(50);
		particles->setZ(-100.f);
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
	if (mElapsed > sf::seconds(0.5f))
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
