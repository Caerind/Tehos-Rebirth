#include "Terrain.hpp"
#include "Sources/Core/Scene.hpp"

Terrain::Terrain(ke::Scene& scene, std::size_t type)
	: ke::Actor(scene)
	, mLayer(nullptr)
	, mShadow(nullptr)
	, mType(type)
{
	setZ(-100.f);
}

Terrain::~Terrain()
{
}

void Terrain::initializeComponents()
{
	mLayer = createComponent<ke::LayerComponent>(&getApplication().getResource<ke::Tileset>("tileset"), sf::Vector2i(16, 12), sf::Vector2i(64, 64));
	attachComponent(mLayer);
	mLayer->setZ(-100.f);

	mShadow = createComponent<ke::SpriteComponent>();
	attachComponent(mShadow);
	mShadow->setTexture("groundShadow");
	mShadow->setTextureRect(sf::IntRect(0, 0, 512, 384));
	mShadow->setScale(2.f, 2.f);

	loadData();
	sf::Vector2i coords;
	for (coords.y = 0; coords.y < mLayer->getSize().y; coords.y++)
	{
		for (coords.x = 0; coords.x < mLayer->getSize().x; coords.x++)
		{
			unsigned int gid = mLayer->getTileId(coords);
			switch (gid)
			{
				case 11: getScene().createActor<Terrain::MapObject>("")->setPosition(coords.x * 64.f + 22.f, coords.y * 64.f + 39.f); break;
				case 12: getScene().createActor<Terrain::MapObject>("")->setPosition(coords.x * 64.f + 42.f, coords.y * 64.f + 19.f); break;
				default: break;
			}
		}
	}
}

std::size_t Terrain::getTerrainType() const
{
	return mType;
}

void Terrain::loadData()
{
	std::string type = "terrain-" + ke::toString(mType);
	ke::Configuration& config = getApplication().getResource<ke::Configuration>("gamedata");
	std::string code = config.getProperty(type + ".code");
	mLayer->loadFromCode(code);
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
