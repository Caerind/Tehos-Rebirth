#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "../Sources/System/StateManager.hpp"
#include "PopUp.hpp"

class MenuState : public ke::State
{
    public:
        typedef std::shared_ptr<MenuState> Ptr;

    public:
        MenuState();
        virtual ~MenuState();

        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time dt);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

		sf::IntRect getTextureRectFromSpell(int spell);

		void loadText(sf::Text& t);

    private:
		sf::Sprite mBackground;

		sf::Text mCrystalText;

		sf::FloatRect mSettingsButton;
		sf::FloatRect mQuitButton;
		sf::FloatRect mPlayButton;

		sf::FloatRect mHero1;
		sf::FloatRect mHero2;
		sf::FloatRect mHero3;
		sf::FloatRect mHero4;
		sf::FloatRect mHeroSkillLife;
		sf::FloatRect mHeroSkill1;
		sf::FloatRect mHeroSkill2;
		sf::FloatRect mHeroSkill3;
		sf::FloatRect mHeroSkill4;

		sf::FloatRect mSoldierSkillLife0;
		sf::FloatRect mSoldierSkillLife1;
		sf::FloatRect mSoldierSkillLife2;
		sf::FloatRect mSoldierSkillDamage0;
		sf::FloatRect mSoldierSkillDamage1;
		sf::FloatRect mSoldierSkillDamage2;

		sf::Text mTextSHL;
		sf::Text mTextSH1;
		sf::Text mTextSH2;
		sf::Text mTextSH3;
		sf::Text mTextSH4;
		sf::Text mTextSS0L;
		sf::Text mTextSS1L;
		sf::Text mTextSS2L;
		sf::Text mTextSS0D;
		sf::Text mTextSS1D;
		sf::Text mTextSS2D;

		int mSpell1;
		int mSpell2;
		sf::Sprite mSpell1Sprite;
		sf::Sprite mSpell2Sprite;


};

#endif // MENUSTATE_HPP

