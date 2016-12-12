#include "Dead.hpp"
#include "States\GameState.hpp"

Dead::Dead(ke::Scene& scene, std::size_t team, std::size_t type)
	: ke::Actor(scene)
	, mSprite(nullptr)
	, mTeam(team)
	, mType(type)
	, mElapsed(sf::Time::Zero)
{
	setZ(-50.f);
}

Dead::~Dead()
{
}

void Dead::initializeComponents()
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
	if (mTeam == 1 && mType == 1)
	{
		mSprite->setPosition(sf::Vector2f(-64.f, -108.f));
	}
	else
	{
		mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	}
	mSprite->addAnimation("dead", type + "-dead");
	mSprite->playAnimation("dead");
}

void Dead::update(sf::Time dt)
{
	mElapsed += dt;
	if (mElapsed >= sf::seconds(2.f))
	{
		float a = (3.f - mElapsed.asSeconds()) * 255;
		mSprite->setColor(sf::Color(255, 255, 255, static_cast<unsigned int>(a)));
	}
	if (mElapsed >= sf::seconds(3.f))
	{
		remove();
	}
}
