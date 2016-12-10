#ifndef KE_JOYSTICK_HPP
#define KE_JOYSTICK_HPP

#include <cmath>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "../Config.hpp"

namespace ke
{

class Joystick : public sf::Transformable
{
    public:
        Joystick();
		
        void setButtonTexture(sf::Texture& texture);
        void setButtonRect(sf::IntRect const& rect);

        void setBackgroundTexture(sf::Texture& texture);
        void setBackgroundRect(sf::IntRect const& rect);

        sf::Vector2f getDelta() const;
        bool isHeld() const;

        void setDeltaMax(float dMax);

		void blockHorizontal(bool block);
		bool isBlockedHorizontal() const;
		void blockVertical(bool block);
		bool isBlockedVertical() const;

        sf::FloatRect getBounds() const;
        bool contains(sf::Vector2f const& pos) const;

        virtual void handleEvent(sf::Event const& event, const sf::Vector2f& position);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());

    protected:
        sf::Sprite mButton;
        sf::Sprite mBackground;
        bool mHeld;
        unsigned int mFingerId;
        float mDeltaMax;
        bool mMouse;
		bool mBlockHorizontal;
		bool mBlockVertical;
};

} // namespace ke

#endif // KE_JOYSTICK_HPP
