#ifndef KE_TILESET_HPP
#define KE_TILESET_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "PropertiesHolder.hpp"
#include "ResourceManager.hpp"

namespace ke
{

class Tileset : public Resource, public PropertiesHolder
{
	public:
		Tileset();
		Tileset(const pugi::xml_node& node, const std::string& relativePath = "");
		virtual ~Tileset();

		TYPE(Tileset)

		bool loadFromTmxNode(const pugi::xml_node& node);
		bool saveToTmxNode(pugi::xml_node& node);

		void setFirstGid(unsigned int id);
		void setTileSize(const sf::Vector2i& tileSize);
		void setSpacing(unsigned int spacing);
		void setMargin(unsigned int margin);
		void setTileCount(unsigned int tileCount);
		void setColumns(unsigned int columns);
		void setRelativePath(const std::string& path);
		void setImageSource(const std::string& source);
		void setImageTransparent(const sf::Color& transparent);

		unsigned int getFirstGid() const;
		const sf::Vector2i& getTileSize() const;
		unsigned int getSpacing() const;
		unsigned int getMargin() const;
		unsigned int getTileCount() const;
		unsigned int getColumns() const;
		const std::string& getRelativePath() const;
		const std::string& getImageSource() const;
		const sf::Color& getImageTransparent() const;

		sf::Texture& getTexture();
		bool possessId(unsigned int gid);
		sf::Vector2i toPos(unsigned int gid);
		sf::IntRect toRect(unsigned int gid);
		unsigned int toId(const sf::Vector2i& pos);

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		unsigned int mFirstGid;
		sf::Vector2i mTileSize;
		unsigned int mSpacing;
		unsigned int mMargin;
		unsigned int mTileCount;
		unsigned int mColumns;
		std::string mRelativePath;
		std::string mImageSource;
		sf::Color mImageTransparent;

		sf::Texture mTexture; // TODO : Improve texture usage

		bool mImageChanged;
};

} // namespace ke

#endif // KE_TILESET_HPP