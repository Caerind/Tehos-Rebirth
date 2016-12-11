#include "Dead.hpp"

Dead::Dead(ke::Scene& scene, std::size_t team, std::size_t type, int gain)
	: ke::Actor(scene)
	, mSprite(nullptr)
    , mMoneySprite(nullptr)
    , mMoneyText(nullptr)
    , mCrystalSprite(nullptr)
    , mCrystalText(nullptr)
	, mTeam(team)
	, mType(type)
	, mElapsed(sf::Time::Zero)
	, mGain(gain)
	, mCrystalGain(0)
{
	int rd = ke::random(0, 100);
	// TODO : SET RANDOM
	if (rd >= 80) // 15%
	{
		mCrystalGain = 1;
	}
	if (rd >= 95) // 5%
	{
		mCrystalGain = 2;
	}
	if (mTeam == 2)
	{
		mCrystalGain = 0;
	}
	if (mCrystalGain > 0 && mTeam != 2)
	{
		// Add Crystal
		ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
		config.setProperty("crystals", config.getPropertyAs<int>("crystals") + mCrystalGain);
	}

	setZ(-20.f);
}

Dead::~Dead()
{
}

void Dead::initializeComponents()
{
	mSprite = createComponent<ke::AnimatorComponent>();
	std::string type = "";
	switch (mTeam)
	{
		case 1: type += "enemy-" + ke::toString(mType); break;
		case 2: type += "soldier-" + ke::toString(mType); break;
		default: break;
	}
	attachComponent(mSprite); 
	if (mTeam == 1 && (mType == 1 || mType == 1))
	{
		mSprite->setPosition(sf::Vector2f(-64.f, -108.f));
	}
	else
	{
		mSprite->setPosition(sf::Vector2f(-32.f, -54.f));
	}
	mSprite->addAnimation("dead", type + "-dead");
	mSprite->playAnimation("dead");

	if (mGain > 0)
	{
		mMoneySprite = createComponent<ke::SpriteComponent>();
		attachComponent(mMoneySprite);
		mMoneySprite->setTexture("fx");
		mMoneySprite->setTextureRect(sf::IntRect(192, 0, 32, 32));
		mMoneySprite->setZ(200.f);
		mMoneySprite->setPosition(0.f, - 12.f - 32.f);

		mMoneyText = createComponent<ke::TextComponent>();
		attachComponent(mMoneyText);
		mMoneyText->setFont("font");
		mMoneyText->setString("+" + ke::toString(mGain));
		mMoneyText->setSize(20);
		mMoneyText->setFillColor(sf::Color::White);
		mMoneyText->setOutlineColor(sf::Color::Black);
		mMoneyText->setOutlineThickness(2.f);
		mMoneyText->setZ(205.f);
		mMoneyText->setPosition(-mMoneyText->getWidth(), - mMoneyText->getHeight() - 12.f - 12.f);
	}

	if (mCrystalGain > 0 && mTeam != 2)
	{
		mCrystalSprite = createComponent<ke::SpriteComponent>();
		attachComponent(mCrystalSprite);
		mCrystalSprite->setTexture("fx");
		mCrystalSprite->setTextureRect(sf::IntRect(224, 0, 32, 32));
		mCrystalSprite->setZ(210.f);
		mCrystalSprite->setPosition(0.f, - 18.f);

		mCrystalText = createComponent<ke::TextComponent>();
		attachComponent(mCrystalText);
		mCrystalText->setFont("font");
		mCrystalText->setString("+" + ke::toString(mCrystalGain));
		mCrystalText->setSize(20);
		mCrystalText->setFillColor(sf::Color::White);
		mCrystalText->setOutlineColor(sf::Color::Black);
		mCrystalText->setOutlineThickness(2.f);
		mCrystalText->setZ(215.f);
		mCrystalText->setPosition(-mCrystalText->getWidth(), - 12.f);
	}
}

void Dead::update(sf::Time dt)
{
	mElapsed += dt;

	if (mMoneyText != nullptr && mMoneySprite != nullptr)
	{
		if (mElapsed < sf::seconds(0.25f))
		{
			mMoneySprite->move(sf::Vector2f(0.f, -200.f * dt.asSeconds()));
			mMoneyText->move(sf::Vector2f(0.f, -200.f * dt.asSeconds()));
		}
		if (mElapsed > sf::seconds(0.6f))
		{
			mMoneySprite->setVisible(false);
			mMoneyText->setVisible(false);
		}
	}

	if (mCrystalText != nullptr && mCrystalSprite != nullptr)
	{
		if (mElapsed < sf::seconds(0.25f))
		{
			mCrystalSprite->move(sf::Vector2f(0.f, -200.f * dt.asSeconds()));
			mCrystalText->move(sf::Vector2f(0.f, -200.f * dt.asSeconds()));
		}
		if (mElapsed > sf::seconds(0.6f))
		{
			mCrystalSprite->setVisible(false);
			mCrystalText->setVisible(false);
		}
	}

	if (mElapsed >= sf::seconds(2.f))
	{
		float a = (3.f - mElapsed.asSeconds()) * 255;
		mSprite->setColor(sf::Color(255, 255, 255, static_cast<unsigned int>(a)));
	}
	if (mElapsed >= sf::seconds(3.f))
	{
		remove();
	}
}
