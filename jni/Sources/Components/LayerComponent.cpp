#include "LayerComponent.hpp"
#include "../Core/Map.hpp"

namespace ke
{

LayerComponent::LayerComponent(Actor& actor)
	: SceneComponent(actor)
	, mVertices(sf::Triangles)
	, mTileset(nullptr)
	, mSize()
	, mTileSize()
	, mOrientation("orthogonal")
	, mStaggerAxis("y")
	, mStaggerIndex("odd")
	, mHexSideLength(0)
	, mOpacity(1.f)
	, mName()
	, mEncoding()
	, mCompression()
{
	create(nullptr);
}

LayerComponent::LayerComponent(Actor& actor, Tileset* tileset, sf::Vector2i const& size, sf::Vector2i const& tileSize, std::string const& orientation, std::string const& staggerAxis, std::string const& staggerIndex, unsigned int hexSideLength)
	: SceneComponent(actor)
	, mVertices(sf::Triangles)
	, mTileset(nullptr)
	, mSize()
	, mTileSize()
	, mOrientation("orthogonal")
	, mStaggerAxis("y")
	, mStaggerIndex("odd")
	, mHexSideLength(0)
	, mOpacity(1.f)
	, mName()
	, mEncoding()
	, mCompression()
{
	create(tileset, size, tileSize, orientation, staggerAxis, staggerIndex, hexSideLength);
}

LayerComponent::~LayerComponent()
{
	onUnregister();
}

bool LayerComponent::updatable() const
{
	return true;
}

bool LayerComponent::renderable() const
{
	return true;
}

sf::Vector2i LayerComponent::worldToCoords(sf::Vector2f const& world)
{
	return Map::worldToCoords(world - getWorldPosition(), mOrientation, mTileSize, mStaggerIndex, mStaggerAxis, mHexSideLength);
}

sf::Vector2f LayerComponent::coordsToWorld(sf::Vector2i const & coords)
{
	return  Map::coordsToWorld(coords, mOrientation, mTileSize, mStaggerIndex, mStaggerAxis, mHexSideLength) + getWorldPosition();
}

bool LayerComponent::loadFromNode(pugi::xml_node const& node, Tileset* tileset, sf::Vector2i const& size, sf::Vector2i const& tileSize, std::string const& orientation, std::string const& staggerAxis, std::string const& staggerIndex, unsigned int hexSideLength)
{
	if (!node)
	{
		return false;
	}
	sf::Vector2f offset;
	for (const pugi::xml_attribute& attr : node.attributes())
	{
		if (attr.name() == std::string("name"))
		{
			mName = attr.as_string();
		}
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
			mOpacity = attr.as_float();
		}
		if (attr.name() == std::string("visible"))
		{
			mVisible = (attr.as_string() == "true");
		}
	}

	setPosition(offset);

	PropertiesHolder::loadFromXml(node);

	if (propertyExist("z"))
	{
		setZ(getPropertyAs<float>("z"));
	}

	pugi::xml_node dataNode = node.child("data");
	if (!dataNode)
	{
		return false;
	}
	for (pugi::xml_attribute attr = dataNode.first_attribute(); attr; attr = attr.next_attribute())
	{
		if (attr.name() == std::string("encoding"))
		{
			mEncoding = attr.as_string();
		}
		if (attr.name() == std::string("compression"))
		{
			mCompression = attr.as_string();
		}
	}

	mTileset = tileset;
	mSize = size;
	mTileSize = tileSize;
	mOrientation = orientation;
	mStaggerAxis = staggerAxis;
	mStaggerIndex = staggerIndex;
	mHexSideLength = hexSideLength;

	updateRender();

	sf::Vector2i coords;
	const unsigned int FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	const unsigned int FLIPPED_VERTICALLY_FLAG = 0x40000000;
	const unsigned int FLIPPED_DIAGONALLY_FLAG = 0x20000000;
	if (mEncoding == "base64")
	{
		std::string data;
		std::stringstream ss;
		ss << dataNode.text().get();
		ss >> data;
		if (!base64_decode(data))
		{
			return false;
		}
		if (mCompression != "")
		{
			if (!decompressString(data))
			{
				return false;
			}
		}
		std::vector<unsigned char> byteVector;
		byteVector.reserve(mSize.x * mSize.y * 4);
		for (std::string::iterator i = data.begin(); i != data.end(); ++i)
		{
			byteVector.push_back(*i);
		}
		for (std::size_t i = 0; i < byteVector.size() - 3; i += 4)
		{
			unsigned int gid = byteVector[i] | byteVector[i + 1] << 8 | byteVector[i + 2] << 16 | byteVector[i + 3] << 24;
			gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
			setTileId(coords, gid);
			coords.x = (coords.x + 1) % mSize.x;
			if (coords.x == 0)
			{
				coords.y++;
			}
		}
	}
	else if (mEncoding == "csv")
	{
		std::string temp(dataNode.text().get());
		std::stringstream data(temp);
		unsigned int gid;
		while (data >> gid)
		{
			if (data.peek() == ',')
			{
				data.ignore();
			}
			gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
			setTileId(coords, gid);
			coords.x = (coords.x + 1) % mSize.x;
			if (coords.x == 0)
			{
				coords.y++;
			}
		}
	}
	else
	{
		for (pugi::xml_node tile = dataNode.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			unsigned int gid = tile.attribute("gid").as_uint();
			gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
			setTileId(coords, gid);
			coords.x = (coords.x + 1) % mSize.x;
			if (coords.x == 0)
			{
				coords.y++;
			}
		}
	}

	return true;
}

void LayerComponent::saveToNode(pugi::xml_node & node)
{
	if (!node)
	{
		return;
	}
	if (mName != "")
	{
		node.append_attribute("name") = mName.c_str();
	}
	if (getPosition() != sf::Vector2f())
	{
		node.append_attribute("offsetx") = getPosition().x;
		node.append_attribute("offsety") = getPosition().y;
	}
	if (mOpacity != 1.f)
	{
		node.append_attribute("opacity") = mOpacity;
	}
	if (!mVisible)
	{
		node.append_attribute("visible") = "false";
	}

	if (getZ() != 0.f)
	{
		setProperty("z", getZ());
	}

	PropertiesHolder::saveToXml(node);

	node.append_attribute("width") = mSize.x;
	node.append_attribute("height") = mSize.y;
	pugi::xml_node dataNode = node.append_child("data");
	if (!dataNode)
	{
		return;
	}
	if (mEncoding != "")
	{
		dataNode.append_attribute("encoding") = mEncoding.c_str();
	}
	if (mCompression != "")
	{
		dataNode.append_attribute("compression") = mCompression.c_str();
	}

	std::string data;
	sf::Vector2i coords;
	if (mEncoding == "base64")
	{
		data.reserve(mSize.x * mSize.y * 4);
		for (coords.y = 0; coords.y < mSize.y; coords.y++)
		{
			for (coords.x = 0; coords.x < mSize.x; coords.x++)
			{
				const std::size_t id = getTileId(coords);
				data.push_back((char)(id));
				data.push_back((char)(id >> 8));
				data.push_back((char)(id >> 16));
				data.push_back((char)(id >> 24));
			}
		}
		if (mCompression != "")
		{
			if (!compressString(data))
			{
				return;
			}
		}
		if (!base64_encode(data))
		{
			return;
		}
		std::string out = "\n   " + data + "\n  ";
		dataNode.text().set(out.c_str());
	}
	else if (mEncoding == "csv")
	{
		data += "\n";
		for (coords.y = 0; coords.y < mSize.y; coords.y++)
		{
			for (coords.x = 0; coords.x < mSize.x; coords.x++)
			{
				data += ke::toString(getTileId(coords)) + ",";
			}
			data += "\n";
		}
		if (data.size() > 2)
		{
			data.erase(data.size() - 2);
			data += "\n  ";
		}
		dataNode.text().set(data.c_str());
	}
	else
	{
		for (coords.y = 0; coords.y < mSize.y; coords.y++)
		{
			for (coords.x = 0; coords.x < mSize.x; coords.x++)
			{
				dataNode.append_child("tile").append_attribute("gid") = getTileId(coords);
			}
		}
	}
}

unsigned int LayerComponent::getTileId(sf::Vector2i const& coords)
{
	if (0 <= coords.x && coords.x < mSize.x && 0 <= coords.y && coords.y < mSize.y)
	{
		sf::Vertex* vertex = &mVertices[(coords.x + coords.y * mSize.x) * 6];
		if (vertex[0].texCoords != vertex[2].texCoords && mTileset != nullptr)
		{
			return mTileset->toId(static_cast<sf::Vector2i>(vertex->texCoords));
		}
	}
	return 0;
}

void LayerComponent::setTileId(sf::Vector2i const& coords, unsigned int id)
{
	if (0 <= coords.x && coords.x < mSize.x && 0 <= coords.y && coords.y < mSize.y && id != 0)
	{
		sf::Vertex* vertex = &mVertices[(coords.x + coords.y * mSize.x) * 6];
		if (mTileset != nullptr)
		{
			sf::Vector2f pos = static_cast<sf::Vector2f>(mTileset->toPos(id));
			vertex[0].texCoords = pos;
			vertex[1].texCoords = sf::Vector2f(pos.x + mTileSize.x, pos.y);
			vertex[2].texCoords = sf::Vector2f(pos.x + mTileSize.x, pos.y + mTileSize.y);
			vertex[4].texCoords = sf::Vector2f(pos.x, pos.y + mTileSize.y);
			vertex[3].texCoords = vertex[2].texCoords;
			vertex[5].texCoords = vertex[0].texCoords;
		}
	}
}

std::string LayerComponent::getCode()
{
	std::string data;
	data.reserve(mSize.x * mSize.y * 4);
	sf::Vector2i coords;
	for (coords.y = 0; coords.y < mSize.y; coords.y++)
	{
		for (coords.x = 0; coords.x < mSize.x; coords.x++)
		{
			unsigned int id = getTileId(coords);
			data.push_back((char)(id));
			data.push_back((char)(id >> 8));
			data.push_back((char)(id >> 16));
			data.push_back((char)(id >> 24));
		}
	}
	if (!compress(data))
	{
		return "";
	}
	return data;
}

bool LayerComponent::loadFromCode(std::string const& code)
{
	sf::Vector2i coords;
	const unsigned int FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	const unsigned int FLIPPED_VERTICALLY_FLAG = 0x40000000;
	const unsigned int FLIPPED_DIAGONALLY_FLAG = 0x20000000;
	std::string data;
	std::stringstream ss;
	ss << code;
	ss >> data;
	if (!decompress(data))
	{
		return false;
	}
	std::vector<unsigned char> byteVector;
	byteVector.reserve(mSize.x * mSize.y * 4);
	for (std::string::iterator i = data.begin(); i != data.end(); ++i)
	{
		byteVector.push_back(*i);
	}
	for (std::size_t i = 0; i < byteVector.size() - 3; i += 4)
	{
		unsigned int gid = byteVector[i] | byteVector[i + 1] << 8 | byteVector[i + 2] << 16 | byteVector[i + 3] << 24;
		gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
		setTileId(coords, gid);
		coords.x = (coords.x + 1) % mSize.x;
		if (coords.x == 0)
		{
			coords.y++;
		}
	}
	return true;
}

void LayerComponent::create(Tileset* tileset, sf::Vector2i const& size, sf::Vector2i const& tileSize, std::string const& orientation, std::string const& staggerAxis, std::string const& staggerIndex, unsigned int hexSideLength)
{
	mTileset = tileset;
	mSize = size;
	mTileSize = tileSize;
	mOrientation = orientation;
	mStaggerAxis = staggerAxis;
	mStaggerIndex = staggerIndex;
	mHexSideLength = hexSideLength;

	updateRender();
}

Tileset* LayerComponent::getTileset() const
{
	return mTileset;
}

void LayerComponent::setTileset(Tileset* tileset)
{
	mTileset = tileset;
	if (mTileset != nullptr)
	{
		updateRender();
	}
}

float LayerComponent::getOpacity() const
{
	return mOpacity;
}

void LayerComponent::setOpacity(float opacity)
{
	mOpacity = opacity;
	sf::Color color = sf::Color(255, 255, 255, static_cast<unsigned char>(255.f * mOpacity));
	std::size_t size = mVertices.getVertexCount();
	for (std::size_t i = 0; i < size; i++)
	{
		mVertices[i].color = color;
	}
}

std::string LayerComponent::getName() const
{
	return mName;
}

void LayerComponent::setName(std::string const& name)
{
	mName = name;
}

const std::string& LayerComponent::getEncoding() const
{
	return mEncoding;
}

void LayerComponent::setEncoding(std::string const& encoding)
{
	mEncoding = encoding;
}

const std::string& LayerComponent::getCompression() const
{
	return mCompression;
}

void LayerComponent::setCompression(std::string const& compression)
{
	mCompression = compression;
}

const sf::Vector2i& LayerComponent::getSize() const
{
	return mSize;
}

void LayerComponent::setSize(sf::Vector2i const& size)
{
	mSize = size;
	if (mSize != sf::Vector2i())
	{
		updateRender();
	}
}

const sf::Vector2i& LayerComponent::getTileSize() const
{
	return mTileSize;
}

void LayerComponent::setTileSize(sf::Vector2i const& tileSize)
{
	mTileSize = tileSize;
	if (mTileSize != sf::Vector2i())
	{
		updateRender();
	}
}

const std::string& LayerComponent::getOrientation() const
{
	return mOrientation;
}

void LayerComponent::setOrientation(std::string const& orientation)
{
	mOrientation = orientation;
	if (mOrientation != "")
	{
		updateRender();
	}
}

const std::string& LayerComponent::getStaggerAxis() const
{
	return mStaggerAxis;
}

void LayerComponent::setStaggerAxis(std::string const& axis)
{
	mStaggerAxis = axis;
	if (mStaggerAxis != "" && (mOrientation == "hexagonal" || mOrientation == "staggered"))
	{
		updateRender();
	}
}

const std::string& LayerComponent::getStaggerIndex() const
{
	return mStaggerIndex;
}

void LayerComponent::setStaggerIndex(std::string const& index)
{
	mStaggerIndex = index;
	if (mStaggerIndex != "" && (mOrientation == "hexagonal" || mOrientation == "staggered"))
	{
		updateRender();
	}
}

unsigned int LayerComponent::getHexSizeLength() const
{
	return mHexSideLength;
}

void LayerComponent::setHexSideLength(unsigned int hexSideLength)
{
	mHexSideLength = hexSideLength;
	if (mHexSideLength != 0 && mOrientation == "hexagonal")
	{
		updateRender();
	}
}

void LayerComponent::updateRender()
{
	if (mTileset == nullptr || mSize.x == 0 || mSize.y == 0 || mTileSize.x == 0 || mTileSize.y == 0)
	{
		return;
	}
	sf::Vector2f texSize = static_cast<sf::Vector2f>(mTileset->getTileSize());
	sf::Color color = sf::Color(255, 255, 255, static_cast<unsigned char>(255.f * mOpacity));
	mVertices.resize(mSize.x * mSize.y * 6);
	sf::Vector2i coords;
	for (coords.x = 0; coords.x < mSize.x; coords.x++)
	{
		for (coords.y = 0; coords.y < mSize.y; coords.y++)
		{
			sf::Vector2f pos = getVertexPosition(coords);
			sf::Vertex* vertex = &mVertices[(coords.x + coords.y * mSize.x) * 6];
			vertex[0].position = pos;
			vertex[1].position = sf::Vector2f(pos.x + texSize.x, pos.y);
			vertex[2].position = sf::Vector2f(pos.x + texSize.x, pos.y + texSize.y);
			vertex[4].position = sf::Vector2f(pos.x, pos.y + texSize.y);
			vertex[3].position = vertex[2].position;
			vertex[5].position = vertex[0].position;
			for (std::size_t i = 0; i < 6; i++)
			{
				vertex[i].color = color;
			}
		}
	}
}

sf::Vector2f LayerComponent::getVertexPosition(sf::Vector2i const& coords)
{
	return Map::coordsToWorld(coords, mOrientation, mTileSize, mStaggerIndex, mStaggerAxis, mHexSideLength);
}

void LayerComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);

	serializer.create("MapData");
	serializer.save("size", mSize);
	serializer.save("tileSize", mTileSize);
	serializer.save("orientation", mOrientation);
	serializer.save("staggerAxis", mStaggerAxis);
	serializer.save("staggerIndex", mStaggerIndex);
	serializer.save("hexSide", mHexSideLength);
	serializer.close();

	serializer.create("LayerData");
	serializer.save("name", mName);
	serializer.save("opacity", mOpacity);
	if (mTileset != nullptr)
	{
		serializer.save("tileset", mTileset->getName());
	}
	serializer.getNode().text().set(getCode().c_str());
	serializer.close();
}

bool LayerComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	if (!serializer.read("MapData"))
	{
		getLog() << ke::Log::Error << ke::Variant("LayerComponent::deserialize : Can't find \"MapData\" in ", getId());
		serializer.end();
		return false;
	}
	if (!serializer.load("size", mSize))
	{
		getLog() << ke::Log::Error << ke::Variant("LayerComponent::deserialize : Can't find \"size\" in ", getId());
		serializer.end();
		return false;
	}
	if (!serializer.load("tileSize", mTileSize))
	{
		getLog() << ke::Log::Error << ke::Variant("LayerComponent::deserialize : Can't find \"tileSize\" in ", getId());
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

	if (!serializer.read("LayerData"))
	{
		getLog() << ke::Log::Error << ke::Variant("LayerComponent::deserialize : Can't find \"LayerData\" in ", getId());
		serializer.end();
		return false;
	}
	if (!serializer.load("name", mName))
	{
		mName = "";
	}
	if (!serializer.load("opacity", mOpacity))
	{
		mOpacity = 1.f;
	}
	std::string tileset;
	if (!serializer.load("tileset", tileset))
	{
		tileset = "";
	}
	if (tileset != "" && getApplication().isResourceLoaded(tileset))
	{
		mTileset = &getApplication().getResource<Tileset>(tileset);
	}
	else
	{
		mTileset = nullptr;
	}
	updateRender();
	loadFromCode(serializer.getNode().text().get());
	serializer.end();

	return true;
}

void LayerComponent::renderCurrent(sf::RenderTarget & target, sf::RenderStates states)
{
	if (mTileset != nullptr)
	{
		states.texture = &mTileset->getTexture();
	}
	target.draw(mVertices, states);
}

} // namespace ke