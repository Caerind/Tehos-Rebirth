#include "Tileset.hpp"

namespace ke
{

Tileset::Tileset() 
	: mFirstGid(1)
	, mTileSize()
	, mSpacing(0)
	, mMargin(0)
	, mTileCount(0)
	, mColumns(0)
	, mRelativePath("")
	, mImageSource()
	, mImageTransparent(sf::Color::Transparent)
	, mImageChanged(true)
{
}

Tileset::Tileset(const pugi::xml_node& node, const std::string& relativePath)
	: mFirstGid(1)
	, mTileSize()
	, mSpacing(0)
	, mMargin(0)
	, mTileCount(0)
	, mColumns(0)
	, mRelativePath(relativePath)
	, mImageSource()
	, mImageTransparent(sf::Color::Transparent)
	, mImageChanged(true)
{
	loadFromTmxNode(node);
}

Tileset::~Tileset()
{
}

bool Tileset::loadFromTmxNode(const pugi::xml_node& node)
{
	for (const pugi::xml_attribute& attr : node.attributes())
	{
		if (attr.name() == std::string("firstgid"))
		{
			mFirstGid = attr.as_uint();
		}
		if (attr.name() == std::string("name"))
		{
			mName = attr.as_string();
		}
		if (attr.name() == std::string("tilewidth"))
		{
			mTileSize.x = attr.as_int();
		}
		if (attr.name() == std::string("tileheight"))
		{
			mTileSize.y = attr.as_int();
		}
		if (attr.name() == std::string("spacing"))
		{
			mSpacing = attr.as_uint();
		}
		if (attr.name() == std::string("margin"))
		{
			mMargin = attr.as_uint();
		}
		if (attr.name() == std::string("tilecount"))
		{
			mTileCount = attr.as_uint();
		}
		if (attr.name() == std::string("columns"))
		{
			mColumns = attr.as_uint();
		}
	}

	pugi::xml_node image = node.child("image");
	if (image)
	{
		for (const pugi::xml_attribute& attr : image.attributes())
		{
			if (attr.name() == std::string("source"))
			{
				mImageSource = attr.as_string();
			}
			if (attr.name() == std::string("trans"))
			{
				mImageTransparent = fromString<sf::Color>(attr.as_string());
			}
		}
	}

	// TODO : Load more data (properties, tmx data, ...)
	//PropertiesHolder::loadFromXml(node);

	mImageChanged = true;
	getTexture();

	setLoaded(true);

	return true;
}

bool Tileset::saveToTmxNode(pugi::xml_node& node)
{
	node.append_attribute("firstgid") = mFirstGid;
	node.append_attribute("name") = mName.c_str();
	node.append_attribute("tilewidth") = mTileSize.x;
	node.append_attribute("tileheight") = mTileSize.y;
	if (mSpacing != 0)
	{
		node.append_attribute("spacing") = mSpacing;
	}
	if (mMargin != 0)
	{
		node.append_attribute("margin") = mMargin;
	}
	node.append_attribute("tilecount") = mTileCount;
	node.append_attribute("columns") = mColumns;

	pugi::xml_node image = node.append_child("image");
	if (mImageSource != "")
	{
		image.append_attribute("source") = mImageSource.c_str();
	}
	if (mImageTransparent != sf::Color::Transparent)
	{
		image.append_attribute("trans") = toString(mImageTransparent).c_str();
	}

	// TODO : Save more data (properties, tmx data, ...)
	// PropertiesHolder::saveToXml(node);

	return true;
}

void Tileset::setFirstGid(unsigned int id)
{
	mFirstGid = id;
}

void Tileset::setTileSize(const sf::Vector2i& tileSize)
{
	mTileSize = tileSize;
}

void Tileset::setSpacing(unsigned int spacing)
{
	mSpacing = spacing;
}

void Tileset::setMargin(unsigned int margin)
{
	mMargin = margin;
}

void Tileset::setTileCount(unsigned int tileCount)
{
	mTileCount = tileCount;
}

void Tileset::setColumns(unsigned int columns)
{
	mColumns = columns;
}

void Tileset::setRelativePath(const std::string& path)
{
	if (path != mRelativePath)
	{
		mRelativePath = path;
		mImageChanged = true;
	}
}

void Tileset::setImageSource(const std::string& source)
{
	if (source != mImageSource)
	{
		mImageSource = source;
		mImageChanged = true;
	}
}

void Tileset::setImageTransparent(const sf::Color& transparent)
{
	mImageTransparent = transparent;
}

unsigned int Tileset::getFirstGid() const
{
	return mFirstGid;
}

const sf::Vector2i& Tileset::getTileSize() const
{
	return mTileSize;
}

unsigned int Tileset::getSpacing() const
{
	return mSpacing;
}

unsigned int Tileset::getMargin() const
{
	return mMargin;
}

unsigned int Tileset::getTileCount() const
{
	return mTileCount;
}

unsigned int Tileset::getColumns() const
{
	return mColumns;
}

const std::string& Tileset::getRelativePath() const
{
	return mRelativePath;
}

const std::string& Tileset::getImageSource() const
{
	return mImageSource;
}

const sf::Color& Tileset::getImageTransparent() const
{
	return mImageTransparent;
}

sf::Texture& Tileset::getTexture()
{
	if (mImageChanged)
	{
		if (mImageTransparent != sf::Color::Transparent)
		{
			sf::Image image;
			image.loadFromFile(mRelativePath + mImageSource);
			image.createMaskFromColor(mImageTransparent);
			mTexture.loadFromImage(image);
		}
		else
		{
			mTexture.loadFromFile(mRelativePath + mImageSource);
		}
		
		mImageChanged = false;
	}
	return mTexture;
}

bool Tileset::possessId(unsigned int gid)
{
	return (mFirstGid <= gid && gid < mFirstGid + mTileCount);
}

sf::Vector2i Tileset::toPos(unsigned int gid)
{
	if (!possessId(gid))
	{
		return sf::Vector2i();
	}
	gid -= mFirstGid;
	return (mColumns > 0) ? sf::Vector2i((gid % mColumns) * (mTileSize.x + mSpacing) + mMargin, (gid / mColumns) * (mTileSize.y + mSpacing) + mMargin) : sf::Vector2i();
}

sf::IntRect Tileset::toRect(unsigned int gid)
{
	if (!possessId(gid))
	{
		return sf::IntRect();
	}
	gid -= mFirstGid;
	return (mColumns > 0) ? sf::IntRect((gid % mColumns) * (mTileSize.x + mSpacing) + mMargin, (gid / mColumns) * (mTileSize.y + mSpacing) + mMargin, mTileSize.x, mTileSize.y) : sf::IntRect();
}

unsigned int Tileset::toId(const sf::Vector2i& pos)
{
	return (mTileSize != sf::Vector2i()) ? 1 + (pos.x - mMargin) / (mTileSize.x + mSpacing) + (pos.y - mMargin) / (mTileSize.y + mSpacing) * mColumns : 0;
}

void Tileset::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	std::string name = node.attribute("name").as_string();
	if (name != "")
	{
		Tileset& tileset = manager->getResource<Tileset>(name);
		tileset.mRelativePath = "";
		for (const pugi::xml_attribute& attr : node.attributes())
		{
			if (attr.name() == std::string("firstgid"))
			{
				tileset.mFirstGid = attr.as_uint();
			}
			if (attr.name() == std::string("name"))
			{
				tileset.mName = attr.as_string();
			}
			if (attr.name() == std::string("tilewidth"))
			{
				tileset.mTileSize.x = attr.as_int();
			}
			if (attr.name() == std::string("tileheight"))
			{
				tileset.mTileSize.y = attr.as_int();
			}
			if (attr.name() == std::string("spacing"))
			{
				tileset.mSpacing = attr.as_uint();
			}
			if (attr.name() == std::string("margin"))
			{
				tileset.mMargin = attr.as_uint();
			}
			if (attr.name() == std::string("tilecount"))
			{
				tileset.mTileCount = attr.as_uint();
			}
			if (attr.name() == std::string("columns"))
			{
				tileset.mColumns = attr.as_uint();
			}
			if (attr.name() == std::string("source"))
			{
				tileset.mImageSource = attr.as_string();
			}
			if (attr.name() == std::string("trans"))
			{
				tileset.mImageTransparent = fromString<sf::Color>(attr.as_string());
			}
		}
		// TODO : Load more data (properties, tmx data, ...)
		tileset.mImageChanged = true;
		tileset.getTexture();
		tileset.setLoaded(true);
	}
}

void Tileset::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Tileset* tileset = dynamic_cast<Tileset*>(resource);
	if (tileset != nullptr)
	{
		node.append_attribute("firstgid") = tileset->mFirstGid;
		node.append_attribute("name") = tileset->mName.c_str();
		node.append_attribute("tilewidth") = tileset->mTileSize.x;
		node.append_attribute("tileheight") = tileset->mTileSize.y;
		if (tileset->mSpacing != 0)
		{
			node.append_attribute("spacing") = tileset->mSpacing;
		}
		if (tileset->mMargin != 0)
		{
			node.append_attribute("margin") = tileset->mMargin;
		}
		node.append_attribute("tilecount") = tileset->mTileCount;
		node.append_attribute("columns") = tileset->mColumns;
		node.append_attribute("source") = (tileset->mRelativePath + tileset->mImageSource).c_str();
		if (tileset->mImageTransparent != sf::Color::Transparent)
		{
			node.append_attribute("trans") = toString(tileset->mImageTransparent).c_str();
		}
		// TODO : Save more data (properties, tmx data, ...)
	}
}

void Tileset::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

} // namespace ke