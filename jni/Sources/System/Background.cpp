#include "Background.hpp"

namespace ke
{

Background::Background()
{
	mSize = sf::Vector2u();
	mRectangle.setFillColor(sf::Color::Black);
	mUsage = Usage::Color;
}

void Background::useColor(const sf::Color& color)
{
	mRectangle.setFillColor(color);
	mUsage = Usage::Color;
	mSize = sf::Vector2u(); // Force update
}

void Background::useScaledTexture(Texture* texture, sf::IntRect rect)
{
	mTexture = texture;
	if (mTexture != nullptr)
	{
		mSprite.setTexture(*mTexture);
		if (rect != sf::IntRect())
		{
			mSprite.setTextureRect(rect);
		}
		mUsage = Usage::Scaled;
	}
	else
	{
		mUsage = Usage::Color;
	}
	mSize = sf::Vector2u(); // Force update
}

void Background::useRepeatedTexture(Texture* texture, sf::IntRect rect)
{
	mTexture = texture;
	if (mTexture != nullptr)
	{
		mSprite.setTexture(*mTexture);
		if (rect != sf::IntRect())
		{
			mSprite.setTextureRect(rect);
		}
		mUsage = Usage::Repeated;
	}
	else
	{
		mUsage = Usage::Color;
	}
	mSize = sf::Vector2u(); // Force update
}

void Background::draw(sf::RenderTarget& target)
{
	sf::Vector2u s = static_cast<sf::Vector2u>(target.getView().getSize());
	s.x++;
	s.y++;
	if (mSize != s)
	{
		mSize = s;
		update();
	}

	sf::Transform viewTransform;
	viewTransform.translate(target.getView().getCenter() - target.getView().getSize() * 0.5f);

	switch (mUsage)
	{
		case Usage::Color: target.draw(mRectangle, viewTransform); break;
		case Usage::Scaled: target.draw(mSprite, viewTransform); break;
		case Usage::Repeated: 
		{
			float width = mSprite.getGlobalBounds().width;
			float height = mSprite.getGlobalBounds().height;
			int sx = (mSize.x / static_cast<int>(width)) + 1;
			int sy = (mSize.y / static_cast<int>(height)) + 1;
			for (int j = 0; j < sy; j++)
			{
				sf::RenderStates states;
				states.transform *= viewTransform;
				states.transform.translate(0.f, j * height);
				for (int i = 0; i < sx; i++)
				{
					target.draw(mSprite, states);
					states.transform.translate(width, 0.f);
				}
			}
		} break;
		default: break;
	}
}

sf::RectangleShape& Background::getRectangle()
{
	return mRectangle;
}

sf::Sprite& Background::getSprite()
{
	return mSprite;
}

const sf::Color& Background::getColor() const
{
	return mRectangle.getFillColor();
}

const sf::IntRect& Background::getTextureRect() const
{
	return mSprite.getTextureRect();
}

std::size_t Background::getUsage() const
{
	return mUsage;
}

std::string Background::getTextureName() const
{
	return (mTexture != nullptr) ? mTexture->getName() : "";
}

void Background::update()
{
	switch (mUsage)
	{
		case Usage::Color: mRectangle.setSize(static_cast<sf::Vector2f>(mSize)); break;
		case Usage::Scaled: mSprite.setScale(static_cast<float>(mSize.x) / mSprite.getGlobalBounds().width, static_cast<float>(mSize.y) / mSprite.getGlobalBounds().height); break;
		case Usage::Repeated: mSprite.setScale(1.f, 1.f); break;
		default: break;
	}
}

} // namespace ke