#include "Terrain.hpp"
#include "Sources/Core/Scene.hpp"

Terrain::Terrain(ke::Scene& scene)
	: ke::Actor(scene)
	, mLayer(nullptr)
	, mShadow(nullptr)
{
	setZ(-200.f);
}

Terrain::~Terrain()
{
}

void Terrain::initializeComponents()
{
	mLayer = createComponent<ke::LayerComponent>(&getApplication().getResource<ke::Tileset>("tileset"), sf::Vector2i(18, 11), sf::Vector2i(64, 64));
	attachComponent(mLayer);
	mLayer->setZ(-100.f);
	mLayer->setPosition(64.f, 0.f);

	mShadow = createComponent<ke::SpriteComponent>();
	attachComponent(mShadow);
	mShadow->setTexture("groundShadow");
	mShadow->setTextureRect(sf::IntRect(0, 0, 640, 360));
	mShadow->setScale(2.f, 2.f);

	sf::Vector2i coords;
	for (coords.y = 0; coords.y < mLayer->getSize().y; coords.y++)
	{
		for (coords.x = 0; coords.x < mLayer->getSize().x; coords.x++)
		{
			unsigned int gid = ke::random(1, 20);
			if (ke::random(1,3) < 3)
			{
				gid = 1;
			}
			else
			{
				if (gid == 11)
				{
					if (coords.x < 2 || coords.y < 2 || coords.x > 15 || coords.y > 8)
					{
						gid = 1;
					}
					else
					{
						getScene().createActor<Terrain::MapObject>("")->setPosition((coords.x + 1) * 64.f + 22.f, coords.y * 64.f + 39.f);
					}
				}
				if (gid == 12)
				{
					if (coords.x < 2 || coords.y < 2 || coords.x > 15 || coords.y > 8)
					{
						gid = 1;
					}
					else
					{
						getScene().createActor<Terrain::MapObject>("")->setPosition((coords.x + 1) * 64.f + 42.f, coords.y * 64.f + 19.f);
					}
				}
			}
			mLayer->setTileId(coords, gid);
		}
	}
}

Terrain::MapObject::MapObject(ke::Scene& scene)
	: ke::Actor(scene)
	, mSprite(nullptr)
{
}

Terrain::MapObject::~MapObject()
{
}

void Terrain::MapObject::initializeComponents()
{
	mSprite = createComponent<ke::SpriteComponent>();
	attachComponent(mSprite);
	mSprite->setTexture(getApplication().getResource<ke::Texture>("fx"));
	mSprite->setTextureRect(sf::IntRect(160,0,32,32));
	mSprite->setPosition(-16.f, -18.f);
}
