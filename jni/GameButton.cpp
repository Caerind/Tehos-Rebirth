#include "GameButton.hpp"

GameButton::GameButton(const std::string& texture)
{
	mSprite.setTexture(ke::Application::getResource<ke::Texture>(texture));
}

void GameButton::setTextureRect(const sf::IntRect& rect)
{
	mSprite.setTextureRect(rect);
}

void GameButton::setPosition(const sf::Vector2f& pos)
{
	mSprite.setPosition(pos);
}

void GameButton::setPosition(float x, float y)
{
	mSprite.setPosition(x, y);
}

sf::FloatRect GameButton::getBounds() const
{
	return mSprite.getGlobalBounds();
}

void GameButton::render(sf::RenderTarget& target)
{
	target.draw(mSprite);
}