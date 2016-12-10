#include "SpriteComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

SpriteComponent::SpriteComponent(Actor& actor) 
	: SceneComponent(actor)
	, mTexture("")
{
}

SpriteComponent::~SpriteComponent()
{
	onUnregister();
}

bool SpriteComponent::renderable() const
{
	return true;
}

void SpriteComponent::setTexture(std::string const& textureName, sf::IntRect const& rect)
{
	if (textureName != "" && getApplication().hasResource(textureName) && getApplication().isResourceLoaded(textureName))
	{
		mTexture = textureName;
		setTexture(getApplication().getResource<Texture>(textureName), rect);
	}
}

void SpriteComponent::setTexture(sf::Texture& texture, sf::IntRect const& rect)
{
    mSprite.setTexture(texture);
    if (rect != sf::IntRect())
    {
        mSprite.setTextureRect(rect);
    }
}

std::string SpriteComponent::getTexture() const
{
	return mTexture;
}

void SpriteComponent::setTextureRect(sf::IntRect const& rect)
{
    mSprite.setTextureRect(rect);
}

sf::IntRect SpriteComponent::getTextureRect() const
{
    return mSprite.getTextureRect();
}

void SpriteComponent::setColor(sf::Color const& color)
{
    mSprite.setColor(color);
}

sf::Color SpriteComponent::getColor() const
{
    return mSprite.getColor();
}

sf::FloatRect SpriteComponent::getLocalBounds()
{
	return getTransform().transformRect(mSprite.getLocalBounds());
}

sf::FloatRect SpriteComponent::getGlobalBounds()
{
	return getWorldTransform().transformRect(mSprite.getLocalBounds());
}

void SpriteComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
	serializer.save("texture", getTexture());
	serializer.save("textureRect", getTextureRect());
	serializer.save("color", getColor());
}

bool SpriteComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	std::string texture;
	if (!serializer.load("texture", texture))
	{
		getLog() << ke::Log::Warning << ke::Variant("SpriteComponent::deserialize : Can't find \"texture\" in ", getId());
		texture = "";
	}
	setTexture(texture);

	sf::IntRect textureRect;
	if (!serializer.load("textureRect", textureRect))
	{
		getLog() << ke::Log::Warning << ke::Variant("SpriteComponent::deserialize : Can't find \"textureRect\" in ", getId());
		texture = "";
	}
	setTextureRect(textureRect);

	sf::Color color;
	if (!serializer.load("color", color))
	{
		getLog() << ke::Log::Warning << ke::Variant("SpriteComponent::deserialize : Can't find \"color\" in ", getId());
		color = sf::Color();
	}
	setColor(color);

	return true;
}

void SpriteComponent::renderCurrent(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mSprite, states);
}

} // namespace ke