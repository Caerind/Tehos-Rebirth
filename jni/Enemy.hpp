#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "AI.hpp"

class Enemy : public AI
{
	public:
		typedef std::shared_ptr<Enemy> Ptr;

		Enemy(ke::Scene& scene, std::size_t type);
		~Enemy();

		std::size_t getTeam() const;

		bool isGiant() const;

	private:
		void loadData();
};

#endif // ENEMY_HPP

