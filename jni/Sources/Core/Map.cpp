#include "Map.hpp"
#include "Scene.hpp"
#include "Pathfinding.hpp"

namespace ke
{

std::vector<sf::Vector2i> Map::getNeighboors(sf::Vector2i const& coords, std::string const& orientation, bool diag, std::string const& staggerIndex, std::string const& staggerAxis)
{
	std::vector<sf::Vector2i> n;
	if (orientation == "orthogonal")
	{
		n.push_back({ coords.x, coords.y - 1 });
		n.push_back({ coords.x, coords.y + 1 });
		n.push_back({ coords.x - 1, coords.y });
		n.push_back({ coords.x + 1, coords.y });
		if (diag)
		{
			n.push_back({ coords.x + 1, coords.y - 1 });
			n.push_back({ coords.x + 1, coords.y + 1 });
			n.push_back({ coords.x - 1, coords.y + 1 });
			n.push_back({ coords.x - 1, coords.y - 1 });
		}
	}
	else if (orientation == "isometric")
	{
		n.push_back({ coords.x - 1, coords.y });
		n.push_back({ coords.x, coords.y - 1 });
		n.push_back({ coords.x + 1, coords.y });
		n.push_back({ coords.x, coords.y + 1 });
		if (diag)
		{
			n.push_back({ coords.x - 1, coords.y - 1 });
			n.push_back({ coords.x + 1, coords.y - 1 });
			n.push_back({ coords.x + 1, coords.y + 1 });
			n.push_back({ coords.x - 1, coords.y + 1 });
		}
	}
	else if (orientation == "staggered")
	{
		if (coords.y % 2 == 0)
		{
			n.push_back({ coords.x - 1, coords.y - 1 });
			n.push_back({ coords.x, coords.y - 1 });
			n.push_back({ coords.x, coords.y + 1 });
			n.push_back({ coords.x - 1, coords.y + 1 });
		}
		else
		{
			n.push_back({ coords.x, coords.y - 1 });
			n.push_back({ coords.x + 1, coords.y - 1 });
			n.push_back({ coords.x + 1, coords.y + 1 });
			n.push_back({ coords.x, coords.y + 1 });
		}
		if (diag)
		{
			n.push_back({ coords.x, coords.y - 2 });
			n.push_back({ coords.x + 1, coords.y });
			n.push_back({ coords.x, coords.y + 2 });
			n.push_back({ coords.x - 1, coords.y });
		}
	}
	else if (orientation == "hexagonal")
	{
		int indexInt = (staggerIndex == "odd") ? 0 : 1;
		if (staggerAxis == "y") // Pointy
		{
			if (coords.y % 2 == indexInt)
			{
				n.push_back({ coords.x - 1, coords.y - 1 });
				n.push_back({ coords.x, coords.y - 1 });
				n.push_back({ coords.x + 1, coords.y });
				n.push_back({ coords.x, coords.y + 1 });
				n.push_back({ coords.x - 1, coords.y + 1 });
				n.push_back({ coords.x - 1, coords.y });
			}
			else
			{
				n.push_back({ coords.x, coords.y - 1 });
				n.push_back({ coords.x + 1, coords.y - 1 });
				n.push_back({ coords.x + 1, coords.y });
				n.push_back({ coords.x + 1, coords.y + 1 });
				n.push_back({ coords.x, coords.y + 1 });
				n.push_back({ coords.x - 1, coords.y });
			}
		}
		else // Flat
		{
			if (coords.x % 2 == indexInt)
			{
				n.push_back({ coords.x - 1, coords.y - 1 });
				n.push_back({ coords.x, coords.y - 1 });
				n.push_back({ coords.x + 1, coords.y - 1 });
				n.push_back({ coords.x + 1, coords.y });
				n.push_back({ coords.x, coords.y + 1 });
				n.push_back({ coords.x - 1, coords.y });
			}
			else
			{
				n.push_back({ coords.x - 1, coords.y });
				n.push_back({ coords.x, coords.y - 1 });
				n.push_back({ coords.x + 1, coords.y });
				n.push_back({ coords.x + 1, coords.y + 1 });
				n.push_back({ coords.x, coords.y + 1 });
				n.push_back({ coords.x - 1, coords.y + 1 });
			}
		}
	}
	return n;
}

sf::Vector2i Map::worldToCoords(sf::Vector2f const& world, std::string const& orientation, sf::Vector2i const& tileSize, std::string const & staggerIndex, std::string const & staggerAxis, unsigned int hexSide)
{
	sf::Vector2i c;
	if (orientation == "orthogonal")
	{
		c.x = (int)world.x / tileSize.x;
		c.y = (int)world.y / tileSize.y;
	}
	else if (orientation == "isometric")
	{
		sf::Vector2f s = static_cast<sf::Vector2f>(tileSize);
		c.x = static_cast<int>(((world.x - s.x * 0.5f) / s.x) + (world.y / s.y));
		c.y = static_cast<int>((world.y / s.y) - ((world.x - s.x * 0.5f) / s.x));
	}
	else if (orientation == "staggered")
	{
		sf::Vector2f halfSize = sf::Vector2f(tileSize.x * 0.5f, tileSize.y * 0.5f);
		sf::Vector2f sector = sf::Vector2f(floor(world.x / halfSize.x), floor(world.y / halfSize.y));
		sf::Vector2f p = world - sf::Vector2f(sector.x * halfSize.x, sector.y * halfSize.y);
		int indexInt = (staggerIndex == "odd") ? 0 : 1;
		if (staggerAxis == "y")
		{
			if (((int)sector.x + (int)sector.y) % 2 == indexInt)
			{
				if (ke::atan2(halfSize.y - p.y, p.x) > 30.f)
				{
					sector.x--;
					sector.y--;
				}
			}
			else
			{
				(ke::atan2(-p.y, p.x) > -30.f) ? sector.y-- : sector.x--;
			}
			c.x = (int)floor(sector.x * 0.5f);
			c.y = (int)sector.y;
		}
		else
		{
			if (((int)sector.x + (int)sector.y) % 2 == indexInt)
			{
				if (ke::atan2(halfSize.x - p.x, p.y) > 30.f)
				{
					sector.x--;
					sector.y--;
				}
			}
			else
			{
				(ke::atan2(-p.x, p.y) > -30.f) ? sector.x-- : sector.y--;
			}
			c.x = (int)sector.x;
			c.y = (int)floor(sector.y * 0.5f);
		}
	}
	else if (orientation == "hexagonal")
	{
		int indexInt = (staggerIndex == "odd") ? 0 : 1;
		if (staggerAxis == "y") // Pointy
		{
			float h = (static_cast<float>(tileSize.y) - static_cast<float>(hexSide)) * 0.5f;
			float r = static_cast<float>(tileSize.x) * 0.5f;
			sf::Vector2f sector = sf::Vector2f(floor(world.x / (2.f * r)), floor(world.y / (h + static_cast<float>(hexSide))));
			sf::Vector2f p = world - sf::Vector2f(sector.x * (2.f * r), sector.y * (h + static_cast<float>(hexSide)));
			if ((int)sector.y % 2 == indexInt) // Zone A
			{
				if (p.y < h) // Zone A top
				{
					if (p.x < r) // Zone A top left
					{
						if (ke::atan2(h - p.y, p.x) > 30.f)
						{
							sector.x--;
							sector.y--;
						}
					}
					else if (ke::atan2(-p.y, p.x - r) > -30.f) // Zone A top right
					{
						sector.y--;
					}
				}
			}
			else // Zone B
			{
				if (p.y < h) // Zone B top
				{
					if (p.x < r) // Zone B top left
					{
						(ke::atan2(-p.y, p.x) > -30.f) ? sector.y-- : sector.x;
					}
					else if (ke::atan2(h - p.y, p.x - r) > 30.f) // Zone B top right
					{
						sector.y--;
					}
				}
				else if (p.x < r) // Zone B down
				{
					sector.x--;
				}
			}
			c.x = (int)sector.x;
			c.y = (int)sector.y;
		}
		else // Flat
		{
			float h = (static_cast<float>(tileSize.x) - static_cast<float>(hexSide)) * 0.5f;
			float r = static_cast<float>(tileSize.y) * 0.5f;
			sf::Vector2f sector = sf::Vector2f(floor(world.x / (h + static_cast<float>(hexSide))), floor(world.y / (2.f * r)));
			sf::Vector2f p = world - sf::Vector2f(sector.x * (h + static_cast<float>(hexSide)), sector.y * (2.f * r));
			if ((int)sector.x % 2 == indexInt) // Zone A
			{
				if (p.x < h) // Zone A top
				{
					if (p.y < r) // Zone A top left
					{
						if (ke::atan2(h - p.x, p.y) > 30.f)
						{
							sector.y--;
							sector.x--;
						}
					}
					else if (ke::atan2(-p.x, p.y - r) > -30.f) // Zone A top right
					{
						sector.x--;
					}
				}
			}
			else // Zone B
			{
				if (p.x < h) // Zone B top
				{
					if (p.y < r) // Zone B top left
					{
						(ke::atan2(-p.x, p.y) > -30.f) ? sector.x-- : sector.y;
					}
					else if (ke::atan2(h - p.x, p.y - r) > 30.f) // Zone B top right
					{
						sector.x--;
					}
				}
				else if (p.y < r) // Zone B down
				{
					sector.y--;
				}
			}
			c.x = (int)sector.x;
			c.y = (int)sector.y;
		}
	}
	return c;
}

sf::Vector2f Map::coordsToWorld(sf::Vector2i const& coords, std::string const& orientation, sf::Vector2i const& tileSize, std::string const& staggerIndex, std::string const& staggerAxis, unsigned int hexSide)
{
	sf::Vector2f p;
	if (orientation == "orthogonal")
	{
		p.x = static_cast<float>(coords.x * tileSize.x);
		p.y = static_cast<float>(coords.y * tileSize.y);
	}
	else if (orientation == "isometric")
	{
		p.x = static_cast<float>((coords.x - coords.y) * (tileSize.x / 2));
		p.y = static_cast<float>((coords.x + coords.y) * (tileSize.y / 2));
	}
	else if (orientation == "staggered")
	{
		int indexInt = (staggerIndex == "odd") ? 0 : 1;
		if (staggerAxis == "y")
		{
			p.x = static_cast<float>(((coords.y % 2) == indexInt) ? coords.x * tileSize.x : (coords.x + 0.5f) * tileSize.x);
			p.y = coords.y * tileSize.y * 0.5f;
		}
		else
		{
			p.x = coords.x * tileSize.x * 0.5f;
			p.y = static_cast<float>(((coords.x % 2) == indexInt) ? coords.y * tileSize.y : (coords.y + 0.5f) * tileSize.y);
		}
	}
	else if (orientation == "hexagonal")
	{
		int indexInt = (staggerIndex == "odd") ? 0 : 1;
		if (staggerAxis == "y") // Pointy
		{
			p.x = static_cast<float>(((coords.y % 2) == indexInt) ? coords.x * tileSize.x : (coords.x + 0.5f) * tileSize.x);
			p.y = coords.y * (tileSize.y + hexSide) * 0.5f;
		}
		else // Flat
		{
			p.x = coords.x * (tileSize.x + hexSide) * 0.5f;
			p.y = static_cast<float>(((coords.x % 2) == indexInt) ? coords.y * tileSize.y : (coords.y + 0.5f) * tileSize.y);
		}
	}
	return p;
}

Map::Map(Scene& scene)
	: Actor(scene)
	, mLayers()
	, mTileset(nullptr)
	, mSize()
	, mTileSize()
	, mOrientation("orthogonal")
	, mStaggerAxis("y")
	, mStaggerIndex("odd")
	, mHexSideLength(0)
	, mObjectFunction()
{
}

bool Map::loadTmxFile(std::string const& filename)
{
	clearLayers();
	if (filename == "")
	{
		return false;
	}
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str()))
	{
		return false;
	}
	std::string path;
	std::size_t found = filename.find_last_of("/");
	if (found != std::string::npos)
	{
		path = filename.substr(0, found + 1);
	}
	else
	{
		found = filename.find_last_of("\\");
		if (found != std::string::npos)
		{
			path = filename.substr(0, found + 1);
		}
		else
		{
			path = "";
		}
	}
	pugi::xml_node map = doc.child("map");
	if (!map)
	{
		return false;
	}
	for (pugi::xml_attribute attr = map.first_attribute(); attr; attr = attr.next_attribute())
	{
		if (attr.name() == std::string("orientation")) mOrientation = attr.as_string();
		if (attr.name() == std::string("width")) mSize.x = attr.as_int();
		if (attr.name() == std::string("height")) mSize.y = attr.as_int();
		if (attr.name() == std::string("tilewidth")) mTileSize.x = attr.as_int();
		if (attr.name() == std::string("tileheight")) mTileSize.y = attr.as_int();
		if (attr.name() == std::string("hexsidelength")) mHexSideLength = attr.as_uint();
		if (attr.name() == std::string("staggeraxis")) mStaggerAxis = attr.as_string();
		if (attr.name() == std::string("staggerindex")) mStaggerIndex = attr.as_string();
	}
	
	PropertiesHolder::loadFromXml(map);

	for (pugi::xml_node tileset : map.children("tileset"))
	{
		std::string tilesetName = tileset.attribute("name").as_string();
		if (getApplication().isResourceLoaded(tilesetName))
		{
			mTileset = &getApplication().getResource<Tileset>(tilesetName);
		}
		else
		{
			mTileset = &getApplication().getResource<Tileset>(tilesetName, tileset, path);
		}
	}

	for (pugi::xml_node layer = map.child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		LayerComponent::Ptr layerPtr = createComponent<LayerComponent>();
		layerPtr->onRegister();
		attachComponent(layerPtr);
		mLayers.push_back(layerPtr);
		layerPtr->loadFromNode(layer, mTileset, mSize, mTileSize, mOrientation, mStaggerAxis, mStaggerIndex, mHexSideLength);
	}

	for (pugi::xml_node imagelayer = map.child("imagelayer"); imagelayer; imagelayer = imagelayer.next_sibling("imagelayer"))
	{
		SpriteComponent::Ptr image = createComponent<SpriteComponent>();
		image->onRegister();
		attachComponent(image);
		mImages.push_back(image);
		sf::Vector2f offset;
		for (const pugi::xml_attribute& attr : imagelayer.attributes())
		{
			if (attr.name() == std::string("offsetx"))
			{
				offset.x = attr.as_float();
			}
			if (attr.name() == std::string("offsety"))
			{
				offset.y = attr.as_float();
			}
			if (attr.name() == std::string("opacity"))
			{
				image->setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * attr.as_float())));
			}
			if (attr.name() == std::string("visible"))
			{
				image->setVisible((attr.value() == "true"));
			}
		}
		image->setPosition(offset);
		pugi::xml_node img = imagelayer.child("image");
		if (img)
		{
			std::string source = img.attribute("source").as_string();
			if (source != "")
			{
				if (!getApplication().isResourceLoaded(source))
				{
					getApplication().getResource<Texture>(source, path + source);
				}
				image->setTexture(source);
			}
		}
		pugi::xml_node properties = imagelayer.child("properties");
		if (properties)
		{
			for (const pugi::xml_node& property : properties.children("property"))
			{
				if (property.attribute("name").as_string() == std::string("z"))
				{
					image->setZ(property.attribute("value").as_float());
				}
			}
		}
	}

	for (pugi::xml_node group : map.children("objectgroup"))
	{
		for (pugi::xml_node object : group.children("object"))
		{
			if (mObjectFunction)
			{
				mObjectFunction(object);
			}
		}
	}

	return true;
}

bool Map::saveTmxFile(std::string const & filename)
{
	if (filename == "")
	{
		return false;
	}
	pugi::xml_document doc;
	pugi::xml_node map = doc.append_child("map");
	map.append_attribute("orientation") = mOrientation.c_str();
	map.append_attribute("width") = mSize.x;
	map.append_attribute("height") = mSize.y;
	map.append_attribute("tilewidth") = mTileSize.x;
	map.append_attribute("tileheight") = mTileSize.y;
	if (mHexSideLength != 0)
	{
		map.append_attribute("hexsidelength") = mHexSideLength;
	}
	if (mOrientation == "staggered" || mOrientation == "hexagonal")
	{
		map.append_attribute("staggeraxis") = mStaggerAxis.c_str();
		map.append_attribute("staggerindex") = mStaggerIndex.c_str();
	}

	PropertiesHolder::saveToXml(map);

	if (mTileset != nullptr)
	{
		pugi::xml_node tileset = map.append_child("tileset");
		mTileset->saveToTmxNode(tileset);
	}

	for (std::size_t i = 0; i < mLayers.size(); i++)
	{
		pugi::xml_node layer = map.append_child("layer");
		mLayers[i]->saveToNode(layer);
	}

	for (std::size_t i = 0; i < mImages.size(); i++)
	{
		std::shared_ptr<SpriteComponent> image = mImages[i];
		pugi::xml_node imagelayer = map.append_child("imagelayer");
		imagelayer.append_attribute("name") = std::string("ImageLayer" + ke::toString(i)).c_str();
		if (image->getPosition() != sf::Vector2f())
		{
			imagelayer.append_attribute("offsetx") = image->getPosition().x;
			imagelayer.append_attribute("offsety") = image->getPosition().y;
		}
		if (image->getColor() != sf::Color(255, 255, 255, 255))
		{
			imagelayer.append_attribute("opacity") = ((float)image->getColor().a) / 255.f;
		}
		if (!image->isVisible())
		{
			imagelayer.append_attribute("visible") = "false";
		}
		if (image->getZ() != 0.f)
		{
			pugi::xml_node prop = imagelayer.append_child("properties");
			pugi::xml_node pty = prop.append_child("property");
			pty.attribute("name") = "z";
			pty.attribute("value") = image->getZ();
		}
		if (image->getTexture() != "")
		{
			pugi::xml_node img = imagelayer.append_child("image");
			img.append_attribute("source") = image->getTexture().c_str();
		}
	}

	doc.save_file(filename.c_str(), " ");
	return true;
}

std::vector<sf::Vector2i> Map::getNeighboors(sf::Vector2i const& coords, bool diag)
{
	return Map::getNeighboors(coords, mOrientation, diag, mStaggerIndex, mStaggerAxis);
}

sf::Vector2i Map::worldToCoords(sf::Vector2f const & world)
{
	return Map::worldToCoords(world - getPosition(), mOrientation, mTileSize, mStaggerIndex, mStaggerAxis, mHexSideLength);
}

sf::Vector2f Map::coordsToWorld(sf::Vector2i const & coords)
{
	return Map::coordsToWorld(coords, mOrientation, mTileSize, mStaggerIndex, mStaggerAxis, mHexSideLength) + getPosition();
}

std::size_t Map::getImageCount()
{
	return mImages.size();
}

std::shared_ptr<SpriteComponent> Map::getImage(std::size_t index)
{
	return mImages[index];
}

void Map::removeImage(std::size_t index)
{
	mImages.erase(mImages.begin() + index);
}

void Map::clearImages()
{
	mImages.clear();
}

std::shared_ptr<LayerComponent> Map::createLayer(std::string const& tilesetName, sf::Vector2i const& size, sf::Vector2i const& tileSize, std::string const& orientation, std::string const& staggerAxis, std::string const& staggerIndex, unsigned int hexSideLength)
{
	std::shared_ptr<LayerComponent> layer = createComponent<LayerComponent>();
	if (layer != nullptr)
	{
		layer->onRegister();
		attachComponent(layer);
		mLayers.push_back(layer);
		layer->create(&getApplication().getResource<Tileset>(tilesetName), size, tileSize, staggerAxis, staggerIndex, orientation, hexSideLength);
	}
	return layer;
}

std::size_t Map::getLayerCount()
{
	return mLayers.size();
}

std::shared_ptr<LayerComponent> Map::getLayer(std::size_t index)
{
	if (0 <= index && index < mLayers.size())
	{
		return mLayers[index];
	}
	return nullptr;
}

std::shared_ptr<LayerComponent> Map::getLayer(std::string const& name)
{
	std::size_t size = mLayers.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (mLayers[i]->getName() == name)
		{
			return mLayers[i];
		}
	}
	return nullptr;
}

bool Map::hasLayer(std::string const& name)
{
	std::size_t size = mLayers.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (mLayers[i]->getName() == name)
		{
			return true;
		}
	}
	return false;
}

void Map::removeLayer(std::size_t index)
{
	if (0 <= index && index < mLayers.size())
	{
		detachComponent(mLayers[index]);
		removeComponent(mLayers[index]);
		mLayers.erase(mLayers.begin() + index);
	}
}

void Map::removeLayer(std::string const & name)
{
	std::size_t size = mLayers.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (mLayers[i]->getName() == name)
		{
			detachComponent(mLayers[i]);
			removeComponent(mLayers[i]);
			mLayers.erase(mLayers.begin() + i);
			size--;
			i--;
		}
	}
}

void Map::clearLayers()
{
	std::size_t size = mLayers.size();
	for (std::size_t i = 0; i < size; i++)
	{
		removeLayer(i);
	}
}

void Map::setTileset(const std::string& name)
{
	if (name != "" && getApplication().isResourceLoaded(name))
	{
		mTileset = &getApplication().getResource<Tileset>(name);
	}
	else
	{
		mTileset = nullptr;
	}
}

Tileset* Map::getTileset()
{
	return mTileset;
}

const sf::Vector2i& Map::getSize() const
{
	return mSize;
}

void Map::setSize(sf::Vector2i const& size)
{
	mSize = size;
	for (std::size_t i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->setSize(size);
	}
}

const sf::Vector2i& Map::getTileSize() const
{
	return mTileSize;
}

void Map::setTileSize(sf::Vector2i const& tileSize)
{
	mTileSize = tileSize;
	for (std::size_t i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->setTileSize(tileSize);
	}
}

const std::string& Map::getOrientation() const
{
	return mOrientation;
}

void Map::setOrientation(std::string const& orientation)
{
	mOrientation = orientation;
	for (std::size_t i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->setOrientation(orientation);
	}
}

const std::string& Map::getStaggerAxis() const
{
	return mStaggerAxis;
}

void Map::setStaggerAxis(std::string const& axis)
{
	mStaggerAxis = axis;
	for (std::size_t i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->setStaggerAxis(axis);
	}
}

const std::string& Map::getStaggerIndex() const
{
	return mStaggerIndex;
}

void Map::setStaggerIndex(std::string const& index)
{
	mStaggerIndex = index;
	for (std::size_t i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->setStaggerIndex(index);
	}
}

unsigned int Map::getHexSizeLength() const
{
	return mHexSideLength;
}

void Map::setHexSideLength(unsigned int hexSideLength)
{
	mHexSideLength = hexSideLength;
	for (std::size_t i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->setHexSideLength(hexSideLength);
	}
}

void Map::setObjectFunction(std::function<void(pugi::xml_node&node)> function)
{
	mObjectFunction = function;
}

void Map::useForPathFinding(bool diag, std::function<bool(const sf::Vector2i& coords)> checker)
{
	PathFinding::setDiag(diag);
	PathFinding::setChecker(checker);
	PathFinding::setOrientation(mOrientation);
	PathFinding::setStaggerAxis(mStaggerAxis);
	PathFinding::setStaggerIndex(mStaggerIndex);
}

void Map::serialize(Serializer& serializer)
{
	Actor::serialize(serializer);

	serializer.create("MapData");
	serializer.save("size", mSize);
	serializer.save("tileSize", mTileSize);
	serializer.save("orientation", mOrientation);
	serializer.save("staggerAxis", mStaggerAxis);
	serializer.save("staggerIndex", mStaggerIndex);
	serializer.save("hexSide", mHexSideLength);
	serializer.close();

	if (mTileset != nullptr)
	{
		serializer.create("Tileset");
		serializer.save("name", mTileset->getName());
		serializer.close();
	}

	std::size_t iSize = mImages.size();
	for (std::size_t i = 0; i < iSize; i++)
	{
		serializeComponent(serializer, mImages[i]);
	}

	std::size_t lSize = mLayers.size();
	for (std::size_t i = 0; i < lSize; i++)
	{
		serializeComponent(serializer, mLayers[i]);
	}
}

bool Map::deserialize(Serializer& serializer)
{
	if (!Actor::deserialize(serializer))
	{
		return false;
	}

	if (!serializer.read("MapData"))
	{
		serializer.end();
		return false;
	}
	if (!serializer.load("size", mSize))
	{
		serializer.end();
		return false;
	}
	if (!serializer.load("tileSize", mTileSize))
	{
		serializer.end();
		return false;
	}
	if (!serializer.load("orientation", mOrientation))
	{
		mOrientation = "orthogonal";
	}
	if (!serializer.load("staggerAxis", mStaggerAxis))
	{
		mStaggerAxis = "y";
	}
	if (!serializer.load("staggerIndex", mStaggerIndex))
	{
		mStaggerIndex = "odd";
	}
	if (!serializer.load("hexSide", mHexSideLength))
	{
		mHexSideLength = 0;
	}
	serializer.end();

	if (serializer.read("Tileset"))
	{
		std::string name;
		if (!serializer.load("name", name))
		{
			name = "";
		}
		setTileset(name);
		serializer.end();
	}

	while (serializer.read("SpriteComponent"))
	{
		SpriteComponent::Ptr image = createComponent<SpriteComponent>();
		if (image != nullptr)
		{
			image->onRegister();
			if (image->deserialize(serializer))
			{
				mImages.push_back(image);
			}
		}
		serializer.end();
	}

	while (serializer.read("LayerComponent"))
	{
		LayerComponent::Ptr layer = createComponent<LayerComponent>();
		if (layer != nullptr)
		{
			layer->onRegister();
			if (layer->deserialize(serializer))
			{
				mLayers.push_back(layer);
			}
		}
		serializer.end();
	}

	return true;
}

} // namespace ke