#include "MyActor.hpp"
#include "Sources/Core/Scene.hpp"

MyActor::MyActor(ke::Scene& scene)
	: ke::Actor(scene)
	, mA(nullptr)
	, mB(nullptr)
	, mC(nullptr)
	, mD(nullptr)
	, mE(nullptr)
	, mCam(nullptr)
	, mMoving(false)
{
}

void MyActor::initializePhysic()
{
	if (mBody == nullptr && mScene.usePhysic() && mScene.getPhysic() != nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(0, 0);
		bodyDef.type = b2_dynamicBody;
		bodyDef.userData = this;
		bodyDef.linearDamping = 0.01f;
		bodyDef.fixedRotation = true;
		mBody = mScene.getPhysic()->createBody(&bodyDef);
	}
}

void MyActor::initializeComponents()
{
	mA = createComponent<ke::NodeComponent>();
	attachComponent(mA);

	mB = createComponent<ke::AnimatorComponent>();
	attachComponent(mB);
	mB->setPosition(-32.f, -32.f);
	mB->addAnimation("idle", "ninja-cat-idle");
	mB->addAnimation("run", "ninja-cat-run");
	mB->playAnimation("idle");

	if (mScene.useLight())
	{
		mC = createComponent<ke::PointLightComponent>();
		attachComponent(mC);
		ke::Texture& texture = getApplication().getResource<ke::Texture>("pointLightTexture");
		mC->setTexture("pointLightTexture");
		mC->setOrigin(sf::Vector2f(texture.getSize().x * 0.5f, texture.getSize().y * 0.5f));
		mC->setColor(sf::Color(200, 200, 10));
		mC->setIntensity(5.f);
	}

	mD = createComponent<ke::InputComponent>();

	if (mBody != nullptr)
	{
		mE = createComponent<ke::PhysicComponent>();
		attachComponent(mE);
		mE->setPoints({ { -10.f, -10.f },{ 10.f, -10.f },{ 10.f, 20.f },{ -10.f, 20.f } });
		mE->setDensity(1.f);
	}

	mCam = createComponent<ke::CameraComponent>();
	attachComponent(mCam);
}

void MyActor::initialize()
{
	mD->bindAction("MoveUp", [&](std::vector<std::string> const& data)
	{
		desiredImpulseY(-240.f);
		return false;
	});
	mD->bindAction("MoveRight", [&](std::vector<std::string> const& data)
	{
		mVel = 160.f;
		return false;
	});
	mD->bindAction("MoveLeft", [&](std::vector<std::string> const& data)
	{
		mVel = -160.f;
		return false;
	});
	mD->bindAction("Light", [&](std::vector<std::string> const& data)
	{
		mC->setOn(!mC->isOn());
		return false;
	});
}

void MyActor::setVelocityX(float velx)
{
	mVel = velx;
}

void MyActor::update(sf::Time dt)
{
	float l = ke::getLength(getVelocity());
	if (l > 10.f && !mMoving)
	{
		mB->playAnimation("run");
		mMoving = true;
	}
	if (l <= 10.f && mMoving)
	{
		mB->playAnimation("idle");
		mMoving = false;
	}

	desiredImpulseX(mVel);
	mVel = 0.f;
}

void MyActor::serialize(ke::Serializer& serializer)
{
	ke::Actor::serialize(serializer);

	serializeComponent(serializer, mA);
	serializeComponent(serializer, mB);

	if (mScene.useLight())
	{
		serializeComponent(serializer, mC);
	}

	serializeComponent(serializer, mD);

	if (mScene.usePhysic())
	{
		serializeComponent(serializer, mE);
	}

	serializeComponent(serializer, mCam);
}

bool MyActor::deserialize(ke::Serializer& serializer)
{
	if (!ke::Actor::deserialize(serializer))
	{
		return false;
	}

	mA = deserializeComponent<ke::NodeComponent>(serializer);

	mB = deserializeComponent<ke::AnimatorComponent>(serializer);
	if (mB == nullptr)
	{
		getLog() << "Shit";
	}

	if (mScene.useLight())
	{
		mC = deserializeComponent<ke::PointLightComponent>(serializer);
	}

	mD = deserializeComponent<ke::InputComponent>(serializer);

	if (mScene.usePhysic())
	{
		mE = deserializeComponent<ke::PhysicComponent>(serializer);
	}

	mCam = deserializeComponent<ke::CameraComponent>(serializer);

	return true;
}
