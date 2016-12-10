#ifndef KE_PHYSICSYSTEM_HPP
#define KE_PHYSICSYSTEM_HPP

#include <functional>
#include <map>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

#include "../ExtLibs/Box2D/Box2D.h"

#include "Actor.hpp"
#include "Component.hpp"

namespace ke
{

class PhysicSystem : public b2Draw, public b2ContactListener
{
	public:
		PhysicSystem();
		~PhysicSystem();

		void reset();

		void disableGravity();
		void setGravity(const sf::Vector2f& gravity = sf::Vector2f(0.f, 9.8f));
		sf::Vector2f getGravity();

		void setPixelsPerMeter(float pixelsPerMeter);
		float getPixelsPerMeter() const;

		void update(sf::Time dt);
		void render(sf::RenderTarget& target);

		b2World* getWorld();
		b2Body* createBody(b2BodyDef* def);
		void destroyBody(b2Body* body);

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void DrawTransform(const b2Transform& xf);
		void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

		void setRenderDebug(bool render);
		bool isRenderingDebug() const;

		void setRenderFlags(sf::Uint32 flags);
		sf::Uint32 getRenderFlags() const;

		typedef std::function<void(Actor* a, Actor* b)> HitFunction;

		void setHitFunction(std::string const& typeA, std::string const& typeB, HitFunction func);

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

	private:
		b2World* mWorld;
		int mVelocityIterations;
		int mPositionIterations;
		bool mRender;
		sf::RenderTarget* mTarget;
		std::map<std::pair<std::string, std::string>, HitFunction> mHitFunctions;
};

class Physic
{
	public:
		static float pixelsPerMeter;
		static Physic conv;
};

inline sf::Vector2f operator* (b2Vec2 const& v, Physic const& c)
{
	return sf::Vector2f(v.x * Physic::pixelsPerMeter, v.y * Physic::pixelsPerMeter);
}

inline sf::Vector2f operator* (Physic const& c, b2Vec2 const& v)
{
	return v * c;
}

inline b2Vec2 operator* (sf::Vector2f const& v, Physic const& c)
{
	return b2Vec2(v.x / Physic::pixelsPerMeter, v.y / Physic::pixelsPerMeter);
}

inline b2Vec2 operator* (Physic const& c, sf::Vector2f const& v)
{
	return v * c;
}

inline sf::Color operator* (b2Color const& color, Physic const& c)
{
	return sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255));
}

} // namespace ke

#endif // KE_PHYSICSYSTEM_HPP
