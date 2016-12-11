#include "Dead.hpp"

Dead::Dead(ke::Scene& scene, std::size_t team, std::size_t type)
	: ke::Actor(scene)
	, mTeam(team)
	, mType(type)
	, mElapsed(sf::Time::Zero)
{
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
	mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	mSprite->addAnimation("dead", type + "-dead");
	mSprite->playAnimation("dead");
}

void Dead::update(sf::Time dt)
{
	mElapsed += dt;
	if (mElapsed > sf::seconds(2.f))
	{
		float a = (3.f - mElapsed.asSeconds()) * 255;
		mSprite->setColor(sf::Color(255, 255, 255, static_cast<unsigned int>(a)));
	}
	if (mElapsed > sf::seconds(3.f))
	{
		remove();
	}
}
