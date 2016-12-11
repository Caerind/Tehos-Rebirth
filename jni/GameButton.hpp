#ifndef GAMEBUTTON_HPP
#define GAMEBUTTON_HPP

#include "Sources/System/Application.hpp"

class GameButton
{
	public:
		GameButton(const std::string& texture);

		void setTextureRect(const sf::IntRect& rect);

		void setPosition(const sf::Vector2f& pos);
		void setPosition(float x, float y);

		sf::FloatRect getBounds() const;

		void render(sf::RenderTarget& target);

	private:
		sf::Sprite mSprite;
};

#endif // GAMEBUTTON_HPP