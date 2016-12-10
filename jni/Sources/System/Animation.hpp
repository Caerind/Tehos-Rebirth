#ifndef KE_ANIMATION_HPP
#define KE_ANIMATION_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

#include "ResourceManager.hpp"

namespace ke
{

class Animation : public Resource
{
	public:
		struct Frame
		{
			Frame();
			Frame(const std::string& textureName, const sf::IntRect& textureRect, sf::Time duration);

			std::string textureName;
			sf::IntRect textureRect;
			sf::Time duration;
		};

	public:
		Animation();
		virtual ~Animation();

		TYPE(Animation)

		void addFrame(const Animation::Frame& frame = Animation::Frame());
		void addFrame(const std::string& textureName, const sf::IntRect& textureRect, sf::Time duration);

		std::size_t getFrameCount() const;

		Animation::Frame& getFrame(std::size_t index);

		void removeFrame(std::size_t index);

		void removeAllFrames();

		sf::Time getDuration() const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		std::vector<Animation::Frame> mFrames;
};

} // namespace ke

#endif // KE_ANIMATION_HPP

