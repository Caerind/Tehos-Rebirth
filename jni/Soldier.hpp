#ifndef SOLDIER_HPP
#define SOLDIER_HPP

#include "AI.hpp"

class Soldier : public AI
{
	public:
		typedef std::shared_ptr<Soldier> Ptr;

		Soldier(ke::Scene& scene, std::size_t soldierType);
		~Soldier();

		void initializeComponents();
		std::size_t getTeam() const;

		std::size_t getSoldierType() const;

		void loadData();

	private:
		void onDirectionChanged();
		void onStartMoving();
		void onStopMoving();

		void updateNoTarget(sf::Time dt);

	private:
		ke::AnimatorComponent::Ptr mSprite;
		std::size_t mSoldierType;
		sf::Vector2f mPath;
};

#endif // SOLDIER_HPP