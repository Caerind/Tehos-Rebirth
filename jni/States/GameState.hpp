#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../Sources/System/StateManager.hpp"
#include "../Sources/Core/Scene.hpp"

#include "../Terrain.hpp"
#include "../Hero.hpp"
#include "../Enemy.hpp"
#include "../Soldier.hpp"
#include "../Pop.hpp"
#include "../GameButton.hpp"
#include "../AIPlayer.hpp"

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

        virtual void onActivate();
        virtual void onDeactivate();

		void toPostGame(std::size_t id);
		void toSettings();

		static int Level;
		static bool FirstGame;
		static sf::FloatRect Bounds;

    private:
		ke::Scene mScene;
		sf::View mView;

		AIPlayer mAI;

		ke::Configuration& mConfig;

		Hero::Ptr mHero;

		int mLevel;
		std::size_t mEnemiesCount;
		std::size_t mSoldiersCount;
		int mSoldierSelected;
		int mMoney;
		sf::Time mMoneyTime;
		
		std::vector<GameButton> mSoldierButtons;
		std::vector<sf::Sprite> mSoldierSprites;
		std::vector<int> mSoldierPrices;

		GameButton mMoneyButton;
		sf::Sprite mMoneySprite;
		sf::Text mMoneyText;

		GameButton mReturnButton;
		GameButton mSettingsButton;

		sf::Text mLevelText;
};

#endif // GAMESTATE_HPP