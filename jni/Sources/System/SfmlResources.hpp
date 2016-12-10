#ifndef KE_SFMLRESOURCES_HPP
#define KE_SFMLRESOURCES_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "ResourceManager.hpp"

namespace ke
{

namespace SfmlResources
{
	
void registerResources(ResourceManager& manager);

} // namespace SfmlResources

class Image : public Resource, public sf::Image
{
	public:
		Image();
		Image(const std::string& filename);
		virtual ~Image();

		TYPE(Image)

		bool loadFromFile(const std::string& filename);
		bool loadFromMemory(const void* data, std::size_t size);
		bool loadFromStream(sf::InputStream& stream);

		bool saveToFile(const std::string& filename = "") const;

		const std::string& getFilename() const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		std::string mFilename;
};

class Texture : public Resource, public sf::Texture
{
	public:
		Texture();
		Texture(const std::string& filename);
		virtual ~Texture();

		TYPE(Texture)

		bool loadFromFile(const std::string& filename, const sf::IntRect& area = sf::IntRect());
		bool loadFromMemory(const void* data, std::size_t size, const sf::IntRect& area = sf::IntRect());
		bool loadFromStream(sf::InputStream& stream, const sf::IntRect& area = sf::IntRect());
		bool loadFromImage(const sf::Image& image, const sf::IntRect& area = sf::IntRect());

		const std::string& getFilename() const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		std::string mFilename;
};

class Font : public Resource, public sf::Font
{
	public:
		Font();
		Font(const std::string& filename);
		virtual ~Font();

		TYPE(Font)

		bool loadFromFile(const std::string& filename);
		bool loadFromMemory(const void* data, std::size_t size);
		bool loadFromStream(sf::InputStream& stream);

		const std::string& getFilename() const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		std::string mFilename;
};

class SoundBuffer : public Resource, public sf::SoundBuffer
{
	public:
		SoundBuffer();
		SoundBuffer(const std::string& filename);
		virtual ~SoundBuffer();

		TYPE(SoundBuffer)

		bool loadFromFile(const std::string& filename);
		bool loadFromMemory(const void* data, std::size_t size);
		bool loadFromStream(sf::InputStream& stream);
		bool loadFromSamples(const sf::Int16* samples, sf::Uint64 sampleCount, unsigned int channelCount, unsigned int sampleRate);

		const std::string& getFilename() const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		std::string mFilename;
};

class Shader : public Resource, public sf::Shader
{
	public:
		Shader();
		// TODO : Add others constructors ?
		virtual ~Shader();

		TYPE(Shader)

		enum Type
		{
			None = 0,
			Vertex = 1 << 0,
			Fragment = 1 << 1,
			Geometry = 1 << 2,
		};

		bool loadFromFile(const std::string& filename, Shader::Type type);
		bool loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
		bool loadFromFile(const std::string& vertexShaderFilename, const std::string& geometryShaderFilename, const std::string& fragmentShaderFilename);

		bool loadFromMemory(const std::string& shader, Shader::Type type);
		bool loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader);
		bool loadFromMemory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);

		bool loadFromStream(sf::InputStream& stream, Shader::Type type);
		bool loadFromStream(sf::InputStream& vertexShaderStream, sf::InputStream& fragmentShaderStream);
		bool loadFromStream(sf::InputStream& vertexShaderStream, sf::InputStream& geometryShaderStream, sf::InputStream& fragmentShaderStream);

		sf::Uint32 getShaderType() const;
		bool isVertex() const;
		bool isFragment() const;
		bool isGeometry() const;

		const std::string& getVertexFilename() const;
		const std::string& getFragmentFilename() const;
		const std::string& getGeometryFilename() const;

		// TODO : Add setUniform & getUniform ?

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		sf::Shader::Type toSfmlShaderType(const ke::Shader::Type& type);

	private:
		sf::Uint32 mType;
		std::string mVertexFilename;
		std::string mFragmentFilename;
		std::string mGeometryFilename;
};

} // namespace ke

#endif // KE_SFMLRESOURCES_HPP
