#include "GameEffect.hpp"

GameEffect::GameEffect(ke::Scene& scene, std::size_t type)
	: ke::Actor(scene)
	, mType(type)
	, mSprite(nullptr)
	, mElapsed(sf::Time::Zero)
{
	setZ(100.f);
}

GameEffect::~GameEffect()
{
}

void GameEffect::initializeComponents()
{
	if (mType == 1) // Heal
	{
		ke::ParticleComponent::Ptr particles = createComponent<ke::ParticleComponent>();
		attachComponent(particles);
		particles->setTexture("fx");
		particles->addTextureRect(sf::IntRect(32 + 14, 14, 4, 4));
		particles->setParticleVelocity(ke::Distributions::project(sf::Vector2f(0.f, -1.f), 15.f, 100.f, 250.f));
		particles->setParticleScale(ke::Distributions::rect(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.25f, 0.25f)));
		particles->setParticleLifetime(ke::Distributions::uniform(sf::seconds(0.3f), sf::seconds(0.4f)));
		particles->setParticleRotationSpeed(ke::Distributions::uniform(0.f, 45.f));
		particles->setParticleColor(ke::Distributions::colorGrade(sf::Color(0, 255, 255), 0.6f, 1.f));
		particles->emitParticles(100);
	}
	if (mType == 2 || mType == 4)
	{
		mSprite = createComponent<ke::SpriteComponent>();
		attachComponent(mSprite);
		mSprite->setTexture("fx");
		if (mType == 2) // Damage
		{
			mSprite->setTextureRect(sf::IntRect(96, 0, 32, 32));
		}
		else if (mType == 4) // Shock
		{
			mSprite->setTextureRect(sf::IntRect(128, 0, 32, 32));
		}
		mSprite->setPosition(sf::Vector2f(-16.f, -32.f));
	}
}

void GameEffect::update(sf::Time dt)
{
	mElapsed += dt;
	if (mElapsed >= sf::seconds(0.4f))
	{
		remove();
	}
	if (mElapsed >= sf::seconds(0.3f))
	{
		if (mSprite != nullptr)
		{
			float a = (0.4f - mElapsed.asSeconds()) * 2550;
			mSprite->setColor(sf::Color(255, 255, 255, static_cast<unsigned int>(a)));
		}
	}
}
