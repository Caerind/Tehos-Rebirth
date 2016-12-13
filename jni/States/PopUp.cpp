#include "PopUp.hpp"
#include "../Sources/System/Application.hpp"

int PopUp::Ref = 0;

PopUp::PopUp() 
	: ke::State()
{
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");

	mWindow.setTexture(getApplication().getResource<ke::Texture>("gui-popup"));
	mWindow.setTextureRect(sf::IntRect(0, 0, 640, 360));
	mWindow.setOrigin(mWindow.getGlobalBounds().width * 0.5f, mWindow.getGlobalBounds().height * 0.5f);
	mWindow.setPosition(640.f, 360.f);

	mMask.setSize(sf::Vector2f(1280.f, 720.f));
	mMask.setFillColor(sf::Color(20, 20, 20, 128));
	
	mText.setFont(getApplication().getResource<ke::Font>("font"));
	mText.setFillColor(sf::Color::White);
	mText.setOutlineColor(sf::Color::Black);
	mText.setOutlineThickness(2.f);
	mText.setCharacterSize(20);
	mText.setPosition(640.f, 200.f);

	mCrystals = config.getPropertyAs<int>("game.crystals");

	switch (PopUp::Ref)
	{
		case 0:
		{
			mDouble = false;
			mPrice = 0;
			mText.setString("");
		} break;
		case 1:
		{
			mDouble = false;
			mPrice = 0;
			mText.setString("");
		} break;
		case 2:
		{
			mDouble = false;
			mPrice = 0;
			mText.setString("");
		} break;
		case 3:
		{
			mDouble = false;
			mPrice = 0;
			mText.setString("");
		} break;
		case 4:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("hero.slife")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour hero's life for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour hero's life now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 5:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("hero.s1")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour hero's heal spell for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour hero's heal now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 6:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("hero.s2")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour hero's damage spell for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour hero's damage now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 7:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("hero.s3")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour hero's slow spell for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour hero's slow now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 8:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("hero.s4")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour hero's shock spell for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour hero's shock now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 9:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("soldier-0.slife")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour 1st minion's life for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour 1st minion's life now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 10:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("soldier-1.slife")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour 2nd minion's life for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour 2nd minion's life now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 11:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("soldier-2.slife")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour 3rd minion's life for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour 3rd minion's life now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 12:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("soldier-0.sdamage")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour 1st minion's damage for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour 1st minion's damage now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 13:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("soldier-1.sdamage")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour 2nd minion's damage for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour 2nd minion's damage now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 14:
		{
			mPrice = static_cast<int>(3 * std::pow(1.3, config.getPropertyAs<int>("soldier-2.sdamage")));
			if (mCrystals >= mPrice)
			{
				mDouble = true;
				mText.setString("Do you want to increase \nyour 3rd minion's damage for " + ke::toString(mPrice) + " ?");
			}
			else
			{
				mDouble = false;
				mText.setString("You can't afford to expand\nyour 3rd minion's damage now...\nPrice is " + ke::toString(mPrice));
			}
		} break;
		case 15:
		{
			mDouble = false;
			mPrice = 0;
			mText.setString("");
		} break;
		case 16:
		{
			mDouble = false;
			mPrice = 0;
			mText.setString("");
		} break;
	}

	mWindow.setTextureRect(sf::IntRect(0, (mDouble) ? 0 : 360, 640, 360));
	mText.setOrigin(mText.getGlobalBounds().width * 0.5f, 0.f);
}

PopUp::~PopUp()
{
}

bool PopUp::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
	{
		sf::Vector2f p = getApplication().getWindow().getPointerPositionView(getApplication().getWindow().getView());
		if (mDouble)
		{
			if (sf::FloatRect(320.f + 28.f, 180.f + 198.f, 265.f, 132.f).contains(p) || !mWindow.getGlobalBounds().contains(p))
			{
				popState();
				getApplication().playSound("select");
			}
			if (sf::FloatRect(320.f + 348.f, 180.f + 198.f, 265.f, 132.f).contains(p))
			{
				okDouble();
				popState();
				getApplication().playSound("select");
			}
		}
		else
		{
			if (!mWindow.getGlobalBounds().contains(p))
			{
				popState();
				getApplication().playSound("select");
			}
			if (sf::FloatRect(320.f + 189.f, 379.f, 265.f, 132.f).contains(p))
			{
				ok();
				popState();
				getApplication().playSound("select");
			}
		}
		
	}
    return false;
}

bool PopUp::update(sf::Time dt)
{
    return false;
}

void PopUp::render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mMask);
	target.draw(mWindow);
	target.draw(mText);
}

void PopUp::okDouble()
{
	mCrystals -= mPrice;
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	config.setProperty("game.crystals", mCrystals);
	switch (PopUp::Ref)
	{
		case 4: // Hero Skill Life Up
		{
			config.setProperty("hero.slife", 1 + config.getPropertyAs<int>("hero.slife"));
		} break;
		case 5: // Hero Skill Spell 1 Up
		{
			config.setProperty("hero.s1", 1 + config.getPropertyAs<int>("hero.s1"));
		} break;
		case 6: // Hero Skill Spell 2 Up
		{
			config.setProperty("hero.s2", 1 + config.getPropertyAs<int>("hero.s2"));
		} break;
		case 7: // Hero Skill Spell 3 Up
		{
			config.setProperty("hero.s3", 1 + config.getPropertyAs<int>("hero.s3"));
		} break;
		case 8: // Hero Skill Spell 4 Up
		{
			config.setProperty("hero.s4", 1 + config.getPropertyAs<int>("hero.s4"));
		} break;
		case 9: // Hero Skill Soldier 0 Life Up
		{
			config.setProperty("soldier-0.slife", 1 + config.getPropertyAs<int>("soldier-0.slife"));
		} break;
		case 10: // Hero Skill Soldier 1 Life Up
		{
			config.setProperty("soldier-1.slife", 1 + config.getPropertyAs<int>("soldier-1.slife"));
		} break;
		case 11: // Hero Skill Soldier 2 Life Up
		{
			config.setProperty("soldier-2.slife", 1 + config.getPropertyAs<int>("soldier-2.slife"));
		} break;
		case 12: // Hero Skill Soldier 0 Damage Up
		{
			config.setProperty("soldier-0.sdamage", 1 + config.getPropertyAs<int>("soldier-0.sdamage"));
		} break;
		case 13: // Hero Skill Soldier 1 Damage Up
		{
			config.setProperty("soldier-1.sdamage", 1 + config.getPropertyAs<int>("soldier-1.sdamage"));
		} break;
		case 14: // Hero Skill Soldier 2 Damage Up
		{
			config.setProperty("soldier-2.sdamage", 1 + config.getPropertyAs<int>("soldier-2.sdamage"));
		} break;
	}
}

void PopUp::ok()
{
}
