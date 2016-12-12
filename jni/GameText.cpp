#include "GameText.hpp"

GameText::GameText(ke::Scene& scene, int amount, int amount2)
	: ke::Actor(scene)
	, mGain(amount)
	, mCrystalGain(amount2)
	, mElapsed(sf::Time::Zero)
{
	setZ(200.f);
}

GameText::~GameText()
{
}

void GameText::initializeComponents()
{
	if (mGain > 0)
	{
		ke::SpriteComponent::Ptr sprite = createComponent<ke::SpriteComponent>();
		attachComponent(sprite);
		sprite->setTexture("fx");
		sprite->setTextureRect(sf::IntRect(192, 0, 32, 32));
		sprite->setZ(200.f);
		sprite->setPosition(0.f, -12.f - 32.f);

		ke::TextComponent::Ptr text = createComponent<ke::TextComponent>();
		attachComponent(text);
		text->setFont("font");
		text->setString("+" + ke::toString(mGain));
		text->setSize(20);
		text->setFillColor(sf::Color::White);
		text->setOutlineColor(sf::Color::Black);
		text->setOutlineThickness(2.f);
		text->setZ(205.f);
		text->setPosition(-text->getWidth(), -text->getHeight() - 12.f - 12.f);
	}

	if (mCrystalGain > 0)
	{
		ke::SpriteComponent::Ptr sprite = createComponent<ke::SpriteComponent>();
		attachComponent(sprite);
		sprite->setTexture("fx");
		sprite->setTextureRect(sf::IntRect(224, 0, 32, 32));
		sprite->setZ(210.f);
		sprite->setPosition(0.f, -18.f);

		ke::TextComponent::Ptr text = createComponent<ke::TextComponent>();
		attachComponent(text);
		text->setFont("font");
		text->setString("+" + ke::toString(mCrystalGain));
		text->setSize(20);
		text->setFillColor(sf::Color::White);
		text->setOutlineColor(sf::Color::Black);
		text->setOutlineThickness(2.f);
		text->setZ(215.f);
		text->setPosition(-text->getWidth(), -12.f);
	}
}

void GameText::update(sf::Time dt)
{
	mElapsed += dt;
	if (mElapsed < sf::seconds(0.25f))
	{
		move(sf::Vector2f(0.f, -200.f * dt.asSeconds()));
	}
	if (mElapsed > sf::seconds(0.6f))
	{
		remove();
	}
}
