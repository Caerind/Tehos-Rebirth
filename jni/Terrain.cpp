#include "Terrain.hpp"
#include "Sources/Core/Scene.hpp"

Terrain::Terrain(ke::Scene& scene, std::size_t type)
	: ke::Actor(scene)
	, mLayer(nullptr)
	, mType(type)
{
	setZ(-100.f);
}

Terrain::~Terrain()
{
}

void Terrain::initializeComponents()
{
	mLayer = createComponent<ke::LayerComponent>(&getApplication().getResource<ke::Tileset>("tileset"), sf::Vector2i(32, 24), sf::Vector2i(32, 32));
	loadData();
	sf::Vector2i coords;
	for (coords.y = 0; coords.y < mLayer->getSize().y; coords.y++)
	{
		for (coords.x = 0; coords.x < mLayer->getSize().x; coords.x++)
		{
			unsigned int gid = mLayer->getTileId(coords);
			switch (gid)
			{
				case 9: getScene().createActor<Terrain::MapObject>("", 19)->setPosition(coords.x * 32.f + 16.f, coords.y * 32.f + 27.f);
				default: break;
			}
		}
	}

	attachComponent(mLayer);
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

Terrain::MapObject::MapObject(ke::Scene& scene, unsigned int gid)
	: ke::Actor(scene)
	, mSprite(nullptr)
	, mTileId(gid)
{
}

Terrain::MapObject::~MapObject()
{
}

void Terrain::MapObject::initializeComponents()
{
	mSprite = createComponent<ke::SpriteComponent>();
	attachComponent(mSprite);
	mSprite->setTexture(getApplication().getResource<ke::Tileset>("tileset").getTexture());
	mSprite->setTextureRect(getApplication().getResource<ke::Tileset>("tileset").toRect(mTileId));
	mSprite->setPosition(-16.f, -27.f);
}
