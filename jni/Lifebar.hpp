#ifndef LIFEBAR_HPP
#define LIFEBAR_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "Sources/System/Application.hpp"

class Lifebar
{
	public:
		Lifebar();
		~Lifebar();

		void setPercent(float percent);

		void render(sf::RenderTarget& target);

		void setPosition(sf::Vector2f pos);

		void setScale(sf::Vector2f scale);

	private:
		sf::Sprite mBack;
		sf::Sprite mTop;
		float mPercent;
};

#endif // LIFEBAR_HPP

