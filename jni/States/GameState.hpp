#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../Sources/System/StateManager.hpp"
#include "../Sources/System/Application.hpp"
#include "../Sources/Core/Scene.hpp"

#include "../Dead.hpp"
#include "../Terrain.hpp"
#include "../Hero.hpp"
#include "../Enemy.hpp"
#include "../Soldier.hpp"
#include "../Pop.hpp"
#include "../AI.hpp"
#include "../AIPlayer.hpp"
#include "../GameText.hpp"

class GameState : public ke::State
{
    public:
        typedef std::shared_ptr<GameState> Ptr;

    public:
        GameState();
        virtual ~GameState();

        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time dt);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

		void newGame(int level);
		void endGame(int id);

		static int Level;
		static bool FirstGame;
		static sf::FloatRect Bounds;
		static sf::FloatRect MenuButton;
		static sf::FloatRect NextButton;

    private:
		ke::Scene* mScene;
		AIPlayer* mAI;
		Hero::Ptr mHero;

		int mLevel;
		int mSoldierSelected;
		int mMoney;
		sf::Time mMoneyTime;

		bool mEnded;
		int mCrystalGained;
		int mResult;
		
		std::vector<sf::Sprite> mSoldierButtons;
		std::vector<sf::Sprite> mSoldierSprites;
		std::vector<int> mSoldierPrices;

		sf::Sprite mMoneyButton;
		sf::Sprite mMoneySprite;
		sf::Text mMoneyText;
		sf::Sprite mReturnButton;
		sf::Sprite mSettingsButton;
		sf::Text mLevelText;

		sf::Time mEndGameTimer;
		sf::Sprite mWindow;
		sf::Text mTextResult;
		sf::Text mTextCrystals;
		sf::Text mTextEnd;
		sf::Text mTextNext;
};

#endif // GAMESTATE_HPP