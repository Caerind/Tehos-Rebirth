#ifndef AI_HPP
#define AI_HPP

#include "Entity.hpp"

class AI : public Entity
{
	public:
		AI(ke::Scene& scene, std::size_t type);
		~AI();

		struct PosData
		{
			std::string id;
			sf::Vector2f pos;
			std::size_t team;
			bool important;
			sf::FloatRect box;
		};
		static std::vector<PosData> FramePosData;

		void initializeComponents();

		void update(sf::Time dt);

		static sf::Time LastHitPlayed;

	protected:
		virtual bool collide(const sf::Vector2f& mvt);
		virtual void moveTo(const sf::Vector2f& dest, sf::Time dt, bool target);
		virtual void stopMoving();

		sf::Vector2f getPath() const;

	protected:
		ke::AnimatorComponent::Ptr mSprite;
		std::string mDirection;
		bool mMoving;

		sf::Vector2f mPath;

		sf::Time mAttackCooldown;
		sf::Time mAttackCooldownMax;

		sf::Time mAttackCast;
		std::string mTarget;

		float mSpeed;
		int mDamage;
};

#endif // AI_HPP