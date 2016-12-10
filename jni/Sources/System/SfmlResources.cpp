#include "SfmlResources.hpp"

namespace ke
{

void SfmlResources::registerResources(ResourceManager& manager)
{
	Image::registerType(manager);
	Texture::registerType(manager);
	Font::registerType(manager);
	SoundBuffer::registerType(manager);
	Shader::registerType(manager);
}

Image::Image()
	: Resource()
	, mFilename("")
{
}

Image::Image(const std::string& filename)
{
	loadFromFile(filename);
}

Image::~Image()
{
}

bool Image::loadFromFile(const std::string& filename)
{
	bool loaded = sf::Image::loadFromFile(filename);
	setLoaded(loaded);
	mFilename = (loaded) ? filename : "";
	return loaded;
}

bool Image::loadFromMemory(const void* data, std::size_t size)
{
	bool loaded = sf::Image::loadFromMemory(data, size);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

bool Image::loadFromStream(sf::InputStream& stream)
{
	bool loaded = sf::Image::loadFromStream(stream);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

bool Image::saveToFile(const std::string& filename) const
{
	return sf::Image::saveToFile((filename == "") ? mFilename : filename);
}

const std::string& Image::getFilename() const
{
	return mFilename;
}

void Image::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	Image& image = manager->getResource<Image>(node.attribute("name").as_string());

	std::string filename = node.attribute("filename").as_string();
	if (filename != "")
	{
		image.loadFromFile(filename);
	}
}

void Image::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Image* image = dynamic_cast<Image*>(resource);
	if (image != nullptr)
	{
		if (image->getFilename() != "")
		{
			node.append_attribute("filename") = image->getFilename().c_str();
		}
	}
}

void Image::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

Texture::Texture()
	: Resource()
	, mFilename("")
{
}

Texture::Texture(const std::string& filename)
{
	loadFromFile(filename);
}

Texture::~Texture()
{
}

bool Texture::loadFromFile(const std::string& filename, const sf::IntRect& area)
{
	bool loaded = sf::Texture::loadFromFile(filename, area);
	setLoaded(loaded);
	mFilename = (loaded) ? filename : "";
	return loaded;
}

bool Texture::loadFromMemory(const void* data, std::size_t size, const sf::IntRect& area)
{
	bool loaded = sf::Texture::loadFromMemory(data, size, area);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

bool Texture::loadFromStream(sf::InputStream& stream, const sf::IntRect& area)
{
	bool loaded = sf::Texture::loadFromStream(stream, area);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

bool Texture::loadFromImage(const sf::Image& image, const sf::IntRect& area)
{
	bool loaded = sf::Texture::loadFromImage(image, area);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

const std::string& Texture::getFilename() const
{
	return mFilename;
}

void Texture::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	Texture& texture = manager->getResource<Texture>(node.attribute("name").as_string());

	std::string filename = node.attribute("filename").as_string();
	if (filename != "")
	{
		texture.loadFromFile(filename);
	}
}

void Texture::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Texture* texture = dynamic_cast<Texture*>(resource);
	if (texture != nullptr)
	{
		if (texture->getFilename() != "")
		{
			node.append_attribute("filename") = texture->getFilename().c_str();
		}
	}
}

void Texture::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

Font::Font()
	: Resource()
	, mFilename("")
{
}

Font::Font(const std::string& filename)
{
	loadFromFile(filename);
}

Font::~Font()
{
}

bool Font::loadFromFile(const std::string& filename)
{
	bool loaded = sf::Font::loadFromFile(filename);
	setLoaded(loaded);
	mFilename = (loaded) ? filename : "";
	return loaded;
}

bool Font::loadFromMemory(const void* data, std::size_t size)
{
	bool loaded = sf::Font::loadFromMemory(data, size);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

bool Font::loadFromStream(sf::InputStream& stream)
{
	bool loaded = sf::Font::loadFromStream(stream);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

const std::string& Font::getFilename() const
{
	return mFilename;
}

void Font::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	Font& font = manager->getResource<Font>(node.attribute("name").as_string());

	std::string filename = node.attribute("filename").as_string();
	if (filename != "")
	{
		font.loadFromFile(filename);
	}
}

void Font::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Font* font = dynamic_cast<Font*>(resource);
	if (font != nullptr)
	{
		if (font->getFilename() != "")
		{
			node.append_attribute("filename") = font->getFilename().c_str();
		}
	}
}

void Font::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

SoundBuffer::SoundBuffer()
	: Resource()
	, mFilename("")
{
}

SoundBuffer::SoundBuffer(const std::string& filename)
{
	loadFromFile(filename);
}

SoundBuffer::~SoundBuffer()
{
}

bool SoundBuffer::loadFromFile(const std::string& filename)
{
	bool loaded = sf::SoundBuffer::loadFromFile(filename);
	setLoaded(loaded);
	mFilename = (loaded) ? filename : "";
	return loaded;
}

bool SoundBuffer::loadFromMemory(const void* data, std::size_t size)
{
	bool loaded = sf::SoundBuffer::loadFromMemory(data, size);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

bool SoundBuffer::loadFromStream(sf::InputStream& stream)
{
	bool loaded = sf::SoundBuffer::loadFromStream(stream);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

bool SoundBuffer::loadFromSamples(const sf::Int16* samples, sf::Uint64 sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
	bool loaded = sf::SoundBuffer::loadFromSamples(samples, sampleCount, channelCount, sampleRate);
	setLoaded(loaded);
	mFilename = "";
	return loaded;
}

const std::string& SoundBuffer::getFilename() const
{
	return mFilename;
}

void SoundBuffer::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	SoundBuffer& soundBuffer = manager->getResource<SoundBuffer>(node.attribute("name").as_string());

	std::string filename = node.attribute("filename").as_string();
	if (filename != "")
	{
		soundBuffer.loadFromFile(filename);
	}
}

void SoundBuffer::writeToXml(Resource* resource, pugi::xml_node& node)
{
	SoundBuffer* soundBuffer = dynamic_cast<SoundBuffer*>(resource);
	if (soundBuffer != nullptr)
	{
		if (soundBuffer->getFilename() != "")
		{
			node.append_attribute("filename") = soundBuffer->getFilename().c_str();
		}
	}
}

void SoundBuffer::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

Shader::Shader()
	: Resource()
	, mType(Shader::Type::None)
	, mVertexFilename("")
	, mFragmentFilename("")
	, mGeometryFilename("")
{
}

Shader::~Shader()
{
}

bool Shader::loadFromFile(const std::string& filename, Shader::Type type)
{
	bool loaded = sf::Shader::loadFromFile(filename, toSfmlShaderType(type));
	setLoaded(loaded);
	if (loaded)
	{
		mType = mType | type;
		switch (type)
		{
			case sf::Shader::Type::Vertex:
			{
				mVertexFilename = filename;
			} break;
			case sf::Shader::Type::Fragment:
			{
				mFragmentFilename = filename;
			} break;
			case sf::Shader::Type::Geometry:
			{
				mGeometryFilename = filename;
			} break;
		}
	}
	return loaded;
}

bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	return loadFromFile(vertexShaderFilename, Shader::Vertex) && loadFromFile(fragmentShaderFilename, Shader::Fragment);
}

bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& geometryShaderFilename, const std::string& fragmentShaderFilename)
{
	return loadFromFile(vertexShaderFilename, fragmentShaderFilename) && loadFromFile(geometryShaderFilename, Shader::Geometry);
}

bool Shader::loadFromMemory(const std::string& shader, Shader::Type type)
{
	bool loaded = sf::Shader::loadFromMemory(shader, toSfmlShaderType(type));
	setLoaded(loaded);
	if (loaded)
	{
		mType = mType | type;
		switch (type)
		{
			case Shader::Type::Vertex:
			{
				mVertexFilename = "";
			} break;
			case Shader::Type::Fragment:
			{
				mFragmentFilename = "";
			} break;
			case Shader::Type::Geometry:
			{
				mGeometryFilename = "";
			} break;
		}
	}
	return loaded;
}

bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
	return loadFromMemory(vertexShader, Shader::Vertex) && loadFromMemory(fragmentShader, Shader::Fragment);
}

bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader)
{
	return loadFromMemory(vertexShader, fragmentShader) && loadFromMemory(geometryShader, Shader::Geometry);
}

bool Shader::loadFromStream(sf::InputStream& stream, Shader::Type type)
{
	bool loaded = sf::Shader::loadFromStream(stream, toSfmlShaderType(type));
	setLoaded(loaded);
	if (loaded)
	{
		mType = mType | type;
		switch (type)
		{
			case Shader::Type::Vertex:
			{
				mVertexFilename = "";
			} break;
			case Shader::Type::Fragment:
			{
				mFragmentFilename = "";
			} break;
			case Shader::Type::Geometry:
			{
				mGeometryFilename = "";
			} break;
		}
	}
	return loaded;
}

bool Shader::loadFromStream(sf::InputStream& vertexShaderStream, sf::InputStream& fragmentShaderStream)
{
	return loadFromStream(vertexShaderStream, Shader::Vertex) && loadFromStream(fragmentShaderStream, Shader::Fragment);
}

bool Shader::loadFromStream(sf::InputStream& vertexShaderStream, sf::InputStream& geometryShaderStream, sf::InputStream& fragmentShaderStream)
{
	return loadFromStream(vertexShaderStream, fragmentShaderStream) && loadFromStream(geometryShaderStream, Shader::Geometry);
}

sf::Uint32 Shader::getShaderType() const
{
	return mType;
}

bool Shader::isVertex() const
{
	return ((mType & Type::Vertex) != 0);
}

bool Shader::isFragment() const
{
	return ((mType & Type::Fragment) != 0);
}

bool Shader::isGeometry() const
{
	return ((mType & Type::Geometry) != 0);
}

const std::string& Shader::getVertexFilename() const
{
	return mVertexFilename;
}

const std::string& Shader::getFragmentFilename() const
{
	return mFragmentFilename;
}

const std::string& Shader::getGeometryFilename() const
{
	return mGeometryFilename;
}

void Shader::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	Shader& shader = manager->getResource<Shader>(node.attribute("name").as_string());
	shader.mType = node.attribute("type").as_uint();

	if (shader.isVertex())
	{
		std::string vertex = node.attribute("vertex").as_string();
		if (vertex != "")
		{
			shader.loadFromFile(vertex, Shader::Vertex);
		}
	}
	if (shader.isFragment())
	{
		std::string fragment = node.attribute("fragment").as_string();
		if (fragment != "")
		{
			shader.loadFromFile(fragment, Shader::Fragment);
		}
	}
	if (shader.isVertex())
	{
		std::string geometry = node.attribute("geometry").as_string();
		if (geometry != "")
		{
			shader.loadFromFile(geometry, Shader::Geometry);
		}
	}
}

void Shader::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Shader* shader = dynamic_cast<Shader*>(resource);
	if (shader != nullptr)
	{
		node.append_attribute("type") = shader->mType;
		if (shader->isVertex() && shader->getVertexFilename() != "")
		{
			node.append_attribute("vertex") = shader->getVertexFilename().c_str();
		}
		if (shader->isFragment() && shader->getFragmentFilename() != "")
		{
			node.append_attribute("fragment") = shader->getFragmentFilename().c_str();
		}
		if (shader->isGeometry() && shader->getGeometryFilename() != "")
		{
			node.append_attribute("geometry") = shader->getGeometryFilename().c_str();
		}
	}
}

void Shader::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

sf::Shader::Type Shader::toSfmlShaderType(const ke::Shader::Type& type)
{
	switch (type)
	{
		case Type::Fragment: return sf::Shader::Fragment;
		case Type::Geometry: return sf::Shader::Geometry;
	}
	return sf::Shader::Vertex;
}

} // namespace ke