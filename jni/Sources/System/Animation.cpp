#include "Animation.hpp"

#include "String.hpp"

namespace ke
{

Animation::Frame::Frame()
	: textureName("")
	, textureRect(sf::IntRect())
	, duration(sf::Time::Zero)
{
}

Animation::Frame::Frame(const std::string& textureName, const sf::IntRect& textureRect, sf::Time duration)
	: textureName(textureName)
	, textureRect(textureRect)
	, duration(duration)
{
}

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::addFrame(const Animation::Frame& frame)
{
	mFrames.push_back(frame);
}

void Animation::addFrame(const std::string& textureName, const sf::IntRect& textureRect, sf::Time duration)
{
	addFrame(Animation::Frame(textureName, textureRect, duration));
}

std::size_t Animation::getFrameCount() const
{
	return mFrames.size();
}

Animation::Frame& Animation::getFrame(std::size_t index)
{
	return mFrames.at(index);
}

void Animation::removeFrame(std::size_t index)
{
	if (0 <= index && index < mFrames.size())
	{
		mFrames.erase(index + mFrames.begin());
	}
}

void Animation::removeAllFrames()
{
	mFrames.clear();
}

sf::Time Animation::getDuration() const
{
	sf::Time duration;
	for (const auto& frame : mFrames)
	{
		duration += frame.duration;
	}
	return duration;
}

void Animation::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	Animation& animation = manager->getResource<Animation>(node.attribute("name").as_string());
	for (pugi::xml_node frame : node.children("Frame"))
	{
		std::string textureName = frame.attribute("textureName").as_string();
		sf::IntRect textureRect = fromString<sf::IntRect>(frame.attribute("textureRect").as_string());
		sf::Time duration = fromString<sf::Time>(frame.attribute("duration").as_string());
		animation.addFrame(textureName, textureRect, duration);
	}
	animation.setLoaded(true);
}

void Animation::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Animation* animation = dynamic_cast<Animation*>(resource);
	if (animation != nullptr)
	{
		for (const auto& frame : animation->mFrames)
		{
			pugi::xml_node frameNode = node.append_child("Frame");
			frameNode.append_attribute("textureName") = frame.textureName.c_str();
			frameNode.append_attribute("textureRect") = toString<sf::IntRect>(frame.textureRect).c_str();
			frameNode.append_attribute("duration") = toString<sf::Time>(frame.duration).c_str();
		}
	}
}

void Animation::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

} // namespace ke