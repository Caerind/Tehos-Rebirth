#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "AI.hpp"

class Enemy : public AI
{
	public:
		typedef std::shared_ptr<Enemy> Ptr;

		Enemy(ke::Scene& scene, std::size_t enemyType);
		~Enemy();

		void initializeComponents();
		std::size_t getTeam() const;

		std::size_t getEnemyType() const;

		void loadData();

		virtual void onDie();

	private:
		void onDirectionChanged();
		void onStartMoving();
		void onStopMoving();
		void onStartAttack();
	
	private:
		ke::AnimatorComponent::Ptr mSprite;
		std::size_t mEnemyType;
};

#endif // ENEMY_HPP

