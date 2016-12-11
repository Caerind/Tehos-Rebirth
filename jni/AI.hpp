#ifndef AI_HPP
#define AI_HPP

#include "Entity.hpp"

class AI : public Entity
{
	public:
		AI(ke::Scene& scene);
		~AI();

		void update(sf::Time dt);
		sf::FloatRect getBounds() const;

		virtual void attack();

	protected:
		virtual void updateNoTarget(sf::Time dt);
		virtual void updateTarget(sf::Time dt);
		virtual void findTarget();
		virtual bool collide(const sf::Vector2f& mvt);
		virtual void moveTo(const sf::Vector2f& dest, sf::Time dt);
		virtual void startMoving();
		virtual void stopMoving();

		virtual void onDirectionChanged();
		virtual void onStartMoving();
		virtual void onStopMoving();
		virtual void onStartAttack();
		virtual void onAttack();

	protected:
		Entity::Ptr mTarget;
		float mBoxSize;
		std::string mDirection;
		bool mMoving;
		std::size_t mTimerAttack;

		sf::Time mAttackCooldown;
		sf::Time mAttackCooldownMax;

		float mDistance;
		float mSpeed;
		int mDamage;
};

#endif // AI_HPP