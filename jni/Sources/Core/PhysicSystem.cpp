#include "PhysicSystem.hpp"

namespace ke
{

float Physic::pixelsPerMeter = 32.f;
Physic Physic::conv;

PhysicSystem::PhysicSystem()
	: mWorld(new b2World(b2Vec2(0.f, 0.f)))
{
	mWorld->SetDebugDraw(this);
	mWorld->SetContactListener(this);
	SetFlags(b2Draw::e_shapeBit);
	mVelocityIterations = 8;
	mPositionIterations = 3;
	mRender = false;
	mTarget = nullptr;
}

PhysicSystem::~PhysicSystem()
{
	delete mWorld;
}

void PhysicSystem::reset()
{
	delete mWorld;
	mWorld = new b2World(b2Vec2(0.f, 0.f));
	mWorld->SetDebugDraw(this);
}

void PhysicSystem::disableGravity()
{
	mWorld->SetGravity(b2Vec2(0.f, 0.f));
}

void PhysicSystem::setGravity(const sf::Vector2f& gravity)
{
	mWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

sf::Vector2f PhysicSystem::getGravity()
{
	return sf::Vector2f(mWorld->GetGravity().x, mWorld->GetGravity().y);
}

void PhysicSystem::setPixelsPerMeter(float pixelsPerMeter)
{
	Physic::pixelsPerMeter = pixelsPerMeter;
}

float PhysicSystem::getPixelsPerMeter() const
{
	return Physic::pixelsPerMeter;
}

void PhysicSystem::update(sf::Time dt)
{
	mWorld->Step(dt.asSeconds(), mVelocityIterations, mPositionIterations);
}

void PhysicSystem::render(sf::RenderTarget& target)
{
	if (mRender)
	{
		mTarget = &target;
		mWorld->DrawDebugData();
	}
}

b2World* PhysicSystem::getWorld()
{
	return mWorld;
}

b2Body* PhysicSystem::createBody(b2BodyDef* def)
{
	if (mWorld != nullptr)
	{
		return mWorld->CreateBody(def);
	}
	return nullptr;
}

void PhysicSystem::destroyBody(b2Body* body)
{
	if (mWorld != nullptr)
	{
		mWorld->DestroyBody(body);
	}
}

void PhysicSystem::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		polygon.setPoint(i, vertices[i] * Physic::conv);
	}
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(color * Physic::conv);
	if (mTarget != nullptr)
	{
		mTarget->draw(polygon);
	}
}

void PhysicSystem::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		polygon.setPoint(i, vertices[i] * Physic::conv);
	}
	polygon.setOutlineThickness(-1.f);
	sf::Color c = color * Physic::conv;
	c.a = 60;
	polygon.setFillColor(c);
	c.a = 255;
	polygon.setOutlineColor(c);
	if (mTarget != nullptr)
	{
		mTarget->draw(polygon);
	}
}

void PhysicSystem::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	sf::CircleShape circle(radius * Physic::pixelsPerMeter);
	circle.setOrigin(radius * Physic::pixelsPerMeter, radius * Physic::pixelsPerMeter);
	circle.setPosition(center * Physic::conv);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(color * Physic::conv);
	if (mTarget != nullptr)
	{
		mTarget->draw(circle);
	}
}

void PhysicSystem::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	sf::CircleShape circle(radius * Physic::pixelsPerMeter);
	circle.setOrigin(radius * Physic::pixelsPerMeter, radius * Physic::pixelsPerMeter);
	circle.setPosition(center * Physic::conv);
	sf::Color c = color * Physic::conv;
	c.a = 60;
	circle.setFillColor(c);
	c.a = 255;
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(c);
	if (mTarget != nullptr)
	{
		mTarget->draw(circle);
		DrawSegment(center, center + radius * axis, color);
	}
}

void PhysicSystem::DrawSegment(const b2Vec2& p1, const b2Vec2 & p2, const b2Color& color)
{
	sf::Vertex line[] =
	{
		sf::Vertex(p1 * Physic::conv, color * Physic::conv),
		sf::Vertex(p2 * Physic::conv, color * Physic::conv)
	};

	if (mTarget != nullptr)
	{
		mTarget->draw(line, 2, sf::Lines);
	}
}

void PhysicSystem::DrawTransform(const b2Transform& xf)
{
	float lineLength = 0.4f;
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetXAxis(), b2Color(1.f, 0.f, 0.f));
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetYAxis(), b2Color(0.f, 1.f, 0.f));
}

void PhysicSystem::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	sf::CircleShape circle(size * Physic::pixelsPerMeter);
	circle.setOrigin(size * Physic::pixelsPerMeter, size * Physic::pixelsPerMeter);
	circle.setPosition(p * Physic::conv);
	circle.setFillColor(color * Physic::conv);
	if (mTarget != nullptr)
	{
		mTarget->draw(circle);
	}
}

void PhysicSystem::setRenderDebug(bool render)
{
	mRender = render;
}

bool PhysicSystem::isRenderingDebug() const
{
	return mRender;
}

void PhysicSystem::setRenderFlags(sf::Uint32 flags)
{
	SetFlags(flags);
}

sf::Uint32 PhysicSystem::getRenderFlags() const
{
	return GetFlags();
}

void PhysicSystem::setHitFunction(std::string const& typeA, std::string const& typeB, HitFunction func)
{
	mHitFunctions.insert(std::make_pair(std::make_pair(typeA, typeB), func));
}

void PhysicSystem::BeginContact(b2Contact* contact)
{
	Component* cA = static_cast<Component*>(contact->GetFixtureA()->GetUserData());
	Component* cB = static_cast<Component*>(contact->GetFixtureB()->GetUserData());

	if (cA != nullptr && cB != nullptr)
	{
		Actor& a = cA->getActor();
		Actor& b = cB->getActor();

		bool foundAB = false;
		auto fct = mHitFunctions.find(std::make_pair(a.getType(), b.getType()));
		if (fct != mHitFunctions.end())
		{
			foundAB = true;
			if (fct->second)
			{
				fct->second(&a, &b);
			}
		}
		if (!foundAB)
		{
			fct = mHitFunctions.find(std::make_pair(b.getType(), a.getType()));
			if (fct != mHitFunctions.end())
			{
				if (fct->second)
				{
					fct->second(&b, &a);
				}
			}
		}
	}
}

void PhysicSystem::EndContact(b2Contact* contact)
{
}

} // namespace ke