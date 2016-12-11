#ifndef AIPLAYER_HPP
#define AIPLAYER_HPP

#include "Sources/Core/Scene.hpp"

#include "Pop.hpp"

class AIPlayer
{
	public:
		AIPlayer(ke::Scene& scene, int level);
		~AIPlayer();

		void update(sf::Time dt);

		void spawnEnemy(std::size_t type);

		void enemyDied();

		bool hasLost() const;

		int getEnemiesLevel() const;
		int getEnemiesCount() const;
		int getEnemiesToSpawn() const;
		int getEnemiesDead() const;

		sf::Time getReduTime() const;

	private:
		int mLevel;
		int mEnemiesLevel; // Enemies total in wave
		int mEnemiesToSpawn; // Enemies that need to be spawn
		int mEnemiesCount; // Enemies currently on the screen
		int mEnemiesDead; // Enemies that died

		bool mStarted;

		std::vector<sf::FloatRect> mSpawnArea;

		ke::Scene& mScene;

		sf::Time mElapsed;
		sf::Time mDuration;

		sf::Time mDelta;
		sf::Time mBaseTime;
};

#endif // AIPLAYER_HPP
