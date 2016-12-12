#include "Lifebar.hpp"

Lifebar::Lifebar()
{
	mPercent = 100.f;

	mBack.setTexture(ke::Application::getResource<ke::Texture>("lifebar"));
	mBack.setTextureRect(sf::IntRect(0, 0, 300, 40));

	mTop.setTexture(ke::Application::getResource<ke::Texture>("lifebar"));
	mTop.setTextureRect(sf::IntRect(0, 40, 300, 40));
}

Lifebar::~Lifebar()
{
}

void Lifebar::setPercent(float percent)
{
	mPercent = percent;
}

void Lifebar::render(sf::RenderTarget& target)
{
	if (mPercent != 100.f)
	{
		target.draw(mBack);

		mTop.setTextureRect(sf::IntRect(0, 40, static_cast<int>(mPercent * 300), 40));

		target.draw(mTop);
	}
}

void Lifebar::setPosition(sf::Vector2f pos)
{
	mBack.setPosition(pos);
	mTop.setPosition(pos);
}

void Lifebar::setRotation(float rotation)
{
	mBack.setRotation(rotation);
	mTop.setRotation(rotation);
}

void Lifebar::setScale(sf::Vector2f scale)
{
	mBack.setScale(scale);
	mTop.setScale(scale);
}
