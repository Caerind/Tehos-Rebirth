#include "Joystick.hpp"
#include "../Config.hpp"

namespace ke
{

Joystick::Joystick()
{
    mHeld = false;
	mBlockHorizontal = false;
	mBlockVertical = false;
}

void Joystick::setButtonTexture(sf::Texture& texture)
{
    mButton.setTexture(texture);
    mButton.setOrigin(mButton.getGlobalBounds().width /2, mButton.getGlobalBounds().height /2);
}

void Joystick::setButtonRect(sf::IntRect const& rect)
{
    mButton.setTextureRect(rect);
    mButton.setOrigin(mButton.getGlobalBounds().width /2, mButton.getGlobalBounds().height /2);
}

void Joystick::setBackgroundTexture(sf::Texture& texture)
{
    mBackground.setTexture(texture);
    mBackground.setOrigin(mBackground.getGlobalBounds().width /2, mBackground.getGlobalBounds().height /2);
}

void Joystick::setBackgroundRect(sf::IntRect const& rect)
{
    mBackground.setTextureRect(rect);
    mBackground.setOrigin(mBackground.getGlobalBounds().width /2, mBackground.getGlobalBounds().height /2);
}

sf::Vector2f Joystick::getDelta() const
{
    sf::Vector2f p = mButton.getPosition();
	if (mBlockHorizontal)
	{
		p.x = 0.f;
	}
	if (mBlockVertical)
	{
		p.y = 0.f;
	}
    float r = std::sqrt(p.x * p.x + p.y * p.y);
    if (r == 0.f)
    {
        return sf::Vector2f();
    }
	return (p / r) * mDeltaMax;
}

bool Joystick::isHeld() const
{
    return mHeld;
}

void Joystick::setDeltaMax(float dMax)
{
    mDeltaMax = dMax;
}

void Joystick::blockHorizontal(bool block)
{
	mBlockHorizontal = block;
}

bool Joystick::isBlockedHorizontal() const
{
	return mBlockHorizontal;
}

void Joystick::blockVertical(bool block)
{
	mBlockVertical = block;
}

bool Joystick::isBlockedVertical() const
{
	return mBlockVertical;
}

sf::FloatRect Joystick::getBounds() const
{
    sf::FloatRect r = mButton.getGlobalBounds();
    r.left += getPosition().x;
    r.top += getPosition().y;
    return r;
}

bool Joystick::contains(sf::Vector2f const& pos) const
{
    return Joystick::getBounds().contains(pos);
}

void Joystick::handleEvent(sf::Event const& event, const sf::Vector2f& position)
{
	if ((event.type == sf::Event::TouchBegan || (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)) && contains(position))
    {
        mHeld = true;
		if (ke::isMobile())
		{
			mFingerId = event.touch.finger;
		}
        mButton.setPosition(0,0);
    }
    if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::TouchEnded && event.touch.finger == mFingerId))
    {
        mHeld = false;
        mButton.setPosition(0,0);
    }
    if ((event.type == sf::Event::MouseMoved || (event.type == sf::Event::TouchMoved && event.touch.finger == mFingerId)) && mHeld)
    {
        sf::Vector2f p = position - getPosition();
		if (mBlockHorizontal)
		{
			p.x = 0.f;
		}
		if (mBlockVertical)
		{
			p.y = 0.f;
		}
        float r = std::sqrt(p.x * p.x + p.y * p.y);
        if (r >= mDeltaMax)
        {
            mButton.setPosition(mDeltaMax * sf::Vector2f(p.x/r,p.y/r));
        }
        else
        {
            mButton.setPosition(p);
        }
    }
}

void Joystick::render(sf::RenderTarget& target, sf::RenderStates states)
{
	states.transform *= getTransform();
    target.draw(mBackground,states);
    target.draw(mButton,states);
}

} // namespace ke