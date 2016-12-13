#include "MenuState.hpp"
#include "../Sources/System/Application.hpp"

MenuState::MenuState() : ke::State()
{
	ke::Font& font = getApplication().getResource<ke::Font>("font");
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");

	getApplication().getWindow().setView(sf::View(sf::FloatRect(0.f, 0.f, 1280.f, 720.f)));
	mBackground.setTexture(getApplication().getResource<ke::Texture>("gui-menu"));
	
	mCrystalText.setFont(font);
	mCrystalText.setFillColor(sf::Color::White);
	mCrystalText.setOutlineColor(sf::Color::Black);
	mCrystalText.setOutlineThickness(2.f);
	mCrystalText.setCharacterSize(20);
	mCrystalText.setPosition(520.f, 668.f);

	mSettingsButton = sf::FloatRect(1016.f, 208.f, 233.f, 102.f);
	mQuitButton = sf::FloatRect(1164.f, 19.f, 92.f, 94.f);
	mPlayButton = sf::FloatRect(945.f, 378.f, 325.f, 329.f);
	mHero1 = sf::FloatRect(22.f, 447.f, 41.f, 41.f);
	mHero2 = sf::FloatRect(22.f, 509.f, 41.f, 41.f);
	mHero3 = sf::FloatRect(22.f, 569.f, 41.f, 41.f);
	mHero4 = sf::FloatRect(22.f, 630.f, 41.f, 41.f);
	mHeroSkillLife = sf::FloatRect(85.f, 382.f, 93.f, 50.f);
	mHeroSkill1 = sf::FloatRect(85.f, 443.f, 93.f, 50.f);
	mHeroSkill2 = sf::FloatRect(85.f, 504.f, 93.f, 50.f);
	mHeroSkill3 = sf::FloatRect(85.f, 565.f, 93.f, 50.f);
	mHeroSkill4 = sf::FloatRect(85.f, 626.f, 93.f, 50.f);
	mSoldierSkillLife0 = sf::FloatRect(452.f, 443.f, 93.f, 50.f);
	//mSoldierSkillLife1 = sf::FloatRect(452.f, 504.f, 93.f, 50.f);
	//mSoldierSkillLife2 = sf::FloatRect(452.f, 565.f, 93.f, 50.f);
	mSoldierSkillDamage0 = sf::FloatRect(648.f, 443.f, 93.f, 50.f);
	//mSoldierSkillDamage1 = sf::FloatRect(648.f, 504.f, 93.f, 50.f);
	//mSoldierSkillDamage2 = sf::FloatRect(648.f, 565.f, 93.f, 50.f);

	mSpell1 = config.getPropertyAs<int>("hero.spell-1");
	mSpell2 = config.getPropertyAs<int>("hero.spell-2");
	mSpell1Sprite.setTexture(getApplication().getResource<ke::Texture>("gui-game"));
	mSpell2Sprite.setTexture(getApplication().getResource<ke::Texture>("gui-game"));
	mSpell1Sprite.setTextureRect(getTextureRectFromSpell(mSpell1));
	mSpell2Sprite.setTextureRect(getTextureRectFromSpell(mSpell2));
	mSpell1Sprite.setPosition(240.f, 450.f);
	mSpell2Sprite.setPosition(240.f, 570.f);

	loadText(mTextSHL);
	mTextSHL.setPosition(115, 397);
	mTextSHL.setString("11");
	loadText(mTextSH1);
	mTextSH1.setPosition(115, 460);
	mTextSH1.setString("11");
	loadText(mTextSH2);
	mTextSH2.setPosition(115, 521);
	mTextSH2.setString("11");
	loadText(mTextSH3);
	mTextSH3.setPosition(115, 583);
	mTextSH3.setString("11");
	loadText(mTextSH4);
	mTextSH4.setPosition(115, 645);
	mTextSH4.setString("11");
	loadText(mTextSS0L);
	mTextSS0L.setPosition(482, 458);
	mTextSS0L.setString("11");
	//loadText(mTextSS1L);
	//mTextSS1L.setPosition(482, 521);
	//mTextSS1L.setString("11");
	//loadText(mTextSS2L);
	//mTextSS2L.setPosition(482, 584);
	//mTextSS2L.setString("11");
	loadText(mTextSS0D);
	mTextSS0D.setPosition(677, 458);
	mTextSS0D.setString("11");
	//loadText(mTextSS1D);
	//mTextSS1D.setPosition(677, 521);
	//mTextSS1D.setString("11");
	//loadText(mTextSS2D);
	//mTextSS2D.setPosition(677, 584);
	//mTextSS2D.setString("11");
}

MenuState::~MenuState()
{
}

bool MenuState::handleEvent(const sf::Event& event)
{
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
	{
		sf::Vector2f p = getApplication().getWindow().getPointerPositionView(getApplication().getWindow().getView());
		if (mQuitButton.contains(p))
		{
			getApplication().playSound("select");
			clearStates();
			getApplication().getResource<ke::Configuration>("gamedata").saveToFile();
			getApplication().close();
		}
		if (mSettingsButton.contains(p))
		{
			getApplication().playSound("select");
			getApplication().getResource<ke::Configuration>("gamedata").saveToFile();
			pushState("SettingsState");
		}
		if (mPlayButton.contains(p))
		{
			getApplication().playSound("select");
			getApplication().getResource<ke::Configuration>("gamedata").saveToFile();
			pushState("PreGameState");
		}
		if (mSpell1Sprite.getGlobalBounds().contains(p))
		{
			config.setProperty("hero.spell-1", 0);
			mSpell1 = 0;
			mSpell1Sprite.setTextureRect(getTextureRectFromSpell(mSpell1));
		}
		if (mSpell2Sprite.getGlobalBounds().contains(p))
		{
			config.setProperty("hero.spell-2", 0);
			mSpell2 = 0;
			mSpell2Sprite.setTextureRect(getTextureRectFromSpell(mSpell2));
		}
		if (mHero1.contains(p))
		{
			if (mSpell1 < 1)
			{
				mSpell1 = 1;
				config.setProperty("hero.spell-1", 1);
				mSpell1Sprite.setTextureRect(getTextureRectFromSpell(mSpell1));
			}
			else if (mSpell2 < 1)
			{
				mSpell2 = 1;
				config.setProperty("hero.spell-2", 1);
				mSpell2Sprite.setTextureRect(getTextureRectFromSpell(mSpell2));
			}
		}
		if (mHero2.contains(p))
		{
			if (mSpell1 < 1)
			{
				mSpell1 = 2;
				config.setProperty("hero.spell-1", 2);
				mSpell1Sprite.setTextureRect(getTextureRectFromSpell(mSpell1));
			}
			else if (mSpell2 < 1)
			{
				mSpell2 = 2;
				config.setProperty("hero.spell-2", 2);
				mSpell2Sprite.setTextureRect(getTextureRectFromSpell(mSpell2));
			}
		}
		if (mHero3.contains(p))
		{
			if (mSpell1 < 1)
			{
				mSpell1 = 3;
				config.setProperty("hero.spell-1", 3);
				mSpell1Sprite.setTextureRect(getTextureRectFromSpell(mSpell1));
			}
			else if (mSpell2 < 1)
			{
				mSpell2 = 3;
				config.setProperty("hero.spell-2", 3);
				mSpell2Sprite.setTextureRect(getTextureRectFromSpell(mSpell2));
			}
		}
		if (mHero4.contains(p))
		{
			if (mSpell1 < 1)
			{
				mSpell1 = 4;
				config.setProperty("hero.spell-1", 4);
				mSpell1Sprite.setTextureRect(getTextureRectFromSpell(mSpell1));
			}
			else if (mSpell2 < 1)
			{
				mSpell2 = 4;
				config.setProperty("hero.spell-2", 4);
				mSpell2Sprite.setTextureRect(getTextureRectFromSpell(mSpell2));
			}
		}
		if (mHeroSkillLife.contains(p))
		{
			PopUp::Ref = 4;
			pushState("PopUp");
		}
		if (mHeroSkill1.contains(p))
		{
			PopUp::Ref = 5;
			pushState("PopUp");
		}
		if (mHeroSkill2.contains(p))
		{
			PopUp::Ref = 6;
			pushState("PopUp");
		}
		if (mHeroSkill3.contains(p))
		{
			PopUp::Ref = 7;
			pushState("PopUp");
		}
		if (mHeroSkill4.contains(p))
		{
			PopUp::Ref = 8;
			pushState("PopUp");
		}
		if (mSoldierSkillLife0.contains(p))
		{
			PopUp::Ref = 9;
			pushState("PopUp");
		}
		if (mSoldierSkillLife1.contains(p))
		{
			PopUp::Ref = 10;
			pushState("PopUp");
		}
		if (mSoldierSkillLife2.contains(p))
		{
			PopUp::Ref = 11;
			pushState("PopUp");
		}
		if (mSoldierSkillDamage0.contains(p))
		{
			PopUp::Ref = 12;
			pushState("PopUp");
		}
		if (mSoldierSkillDamage1.contains(p))
		{
			PopUp::Ref = 13;
			pushState("PopUp");
		}
		if (mSoldierSkillDamage2.contains(p))
		{
			PopUp::Ref = 14;
			pushState("PopUp");
		}
	}

    return true;
}

bool MenuState::update(sf::Time dt)
{
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	mCrystalText.setString(config.getProperty("game.crystals"));
	mTextSHL.setString(config.getProperty("hero.slife"));
	mTextSH1.setString(config.getProperty("hero.s1"));
	mTextSH2.setString(config.getProperty("hero.s2"));
	mTextSH3.setString(config.getProperty("hero.s3"));
	mTextSH4.setString(config.getProperty("hero.s4"));
	mTextSS0L.setString(config.getProperty("soldier-0.slife"));
	//mTextSS1L.setString(config.getProperty("soldier-1.slife"));
	//mTextSS2L.setString(config.getProperty("soldier-2.slife"));
	mTextSS0D.setString(config.getProperty("soldier-0.sdamage"));
	//mTextSS1D.setString(config.getProperty("soldier-1.sdamage"));
	//mTextSS2D.setString(config.getProperty("soldier-2.sdamage"));
	return true;
}

void MenuState::render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mBackground);
	target.draw(mCrystalText);
	target.draw(mSpell1Sprite);
	target.draw(mSpell2Sprite);

	target.draw(mTextSHL);
	target.draw(mTextSH1);
	target.draw(mTextSH2);
	target.draw(mTextSH3);
	target.draw(mTextSH4);
	target.draw(mTextSS0L);
	//target.draw(mTextSS1L);
	//target.draw(mTextSS2L);
	target.draw(mTextSS0D);
	//target.draw(mTextSS1D);
	//target.draw(mTextSS2D);
}

sf::IntRect MenuState::getTextureRectFromSpell(int spell)
{
	if (spell < 1)
	{
		return sf::IntRect(0, 0, 92, 92);
	}
	else
	{
		return sf::IntRect(460 + 92 * spell, 0, 92, 92);
	}
}

void MenuState::loadText(sf::Text& t)
{
	t.setFont(getApplication().getResource<ke::Font>("font"));
	t.setCharacterSize(20);
	t.setFillColor(sf::Color::White);
	t.setOutlineThickness(2.f);
	t.setOutlineColor(sf::Color::Black);
}
