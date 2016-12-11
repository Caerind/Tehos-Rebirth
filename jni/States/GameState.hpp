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

    private:
		ke::Scene mScene;
		sf::View mView;

		Terrain::Ptr mTerrain;
		Hero::Ptr mHero;

		std::size_t mEnemiesCount;
		std::size_t mSoldiersCount;
		int mSoldierSelected;
		
		std::vector<GameButton> mSoldierButtons;
};

#endif // GAMESTATE_HPP