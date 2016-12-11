#include "TextComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

TextComponent::TextComponent(Actor& actor)
	: SceneComponent(actor)
{
}

TextComponent::~TextComponent()
{
	onUnregister();
}

bool TextComponent::renderable() const
{
	return true;
}

void TextComponent::setFont(std::string const& font)
{
	if (font != "" && getApplication().hasResource(font) && getApplication().isResourceLoaded(font))
	{
		mFont = font;
		mText.setFont(getApplication().getResource<ke::Font>(font));
	}
}

void TextComponent::setFont(sf::Font& font)
{
	mText.setFont(font);
}

void TextComponent::setSize(unsigned int size)
{
	mText.setCharacterSize(size);
}

void TextComponent::setString(std::string const& text)
{
	mText.setString(text);
}

void TextComponent::setFillColor(sf::Color const& color)
{
	mText.setFillColor(color);
}

void TextComponent::setOutlineColor(sf::Color const& color)
{
	mText.setOutlineColor(color);
}

void TextComponent::setOutlineThickness(float thickness)
{
	mText.setOutlineThickness(thickness);
}

std::string TextComponent::getFont() const
{
	return mFont;
}

unsigned int TextComponent::getSize() const
{
	return mText.getCharacterSize();
}

std::string TextComponent::getString() const
{
	return mText.getString().toAnsiString();
}

sf::Color TextComponent::getFillColor() const
{
	return mText.getFillColor();
}

sf::Color TextComponent::getOutlineColor() const
{
	return mText.getOutlineColor();
}

float TextComponent::getOutlineThickness() const
{
	return mText.getOutlineThickness();
}

float TextComponent::getWidth() const
{
	return mText.getGlobalBounds().width;
}

float TextComponent::getHeight() const
{
	return mText.getGlobalBounds().height;
}

void TextComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
	serializer.save("font", getFont());
	serializer.save("size", getSize());
	serializer.save("string", getString());
	serializer.save("fillColor", getFillColor());
	serializer.save("outColor", getOutlineColor());
	serializer.save("outThick", getOutlineThickness());
}

bool TextComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	std::string font;
	if (!serializer.load("font", font))
	{
		getLog() << ke::Log::Warning << ke::Variant("TextComponent::deserialize : Can't find \"font\" in ", getId());
		font = "";
	}
	setFont(font);

	unsigned int size;
	if (!serializer.load("size", size))
	{
		getLog() << ke::Log::Warning << ke::Variant("TextComponent::deserialize : Can't find \"size\" in ", getId());
		size = 20;
	}
	setSize(size);
	
	std::string string;
	if (!serializer.load("string", string))
	{
		getLog() << ke::Log::Warning << ke::Variant("TextComponent::deserialize : Can't find \"string\" in ", getId());
		string = "";
	}
	setString(string);

	sf::Color fillColor;
	if (!serializer.load("fillColor", fillColor))
	{
		getLog() << ke::Log::Warning << ke::Variant("TextComponent::deserialize : Can't find \"fillColor\" in ", getId());
		fillColor = sf::Color();
	}
	setFillColor(fillColor);

	sf::Color outColor;
	if (!serializer.load("outColor", outColor))
	{
		getLog() << ke::Log::Warning << ke::Variant("TextComponent::deserialize : Can't find \"outColor\" in ", getId());
		outColor = sf::Color();
	}
	setOutlineColor(outColor);

	float outThick;
	if (!serializer.load("outThick", outThick))
	{
		getLog() << ke::Log::Warning << ke::Variant("TextComponent::deserialize : Can't find \"outThick\" in ", getId());
		outThick = 0.f;
	}
	setOutlineThickness(outThick);

	return true;
}

void TextComponent::renderCurrent(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mText, states);
}

} // namespace ke