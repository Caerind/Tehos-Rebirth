#include "AnimatorComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

AnimatorComponent::AnimatorComponent(Actor& actor)
	: SceneComponent(actor)
	, mPlaying(false)
	, mSprite()
	, mAnimations()
	, mActualTexture()
	, mActualAnimation()
	, mActualFrame()
	, mTimeElapsed()
{
}

AnimatorComponent::~AnimatorComponent()
{
	onUnregister();
}

bool AnimatorComponent::updatable() const
{
	return true;
}

bool AnimatorComponent::renderable() const
{
	return true;
}

std::size_t AnimatorComponent::getAnimationCount() const
{
	return mAnimations.size();
}

void AnimatorComponent::addAnimation(const std::string& name, const std::string& animationName)
{
	mAnimations[name] = getApplication().getResource<Animation>(animationName);
}

void AnimatorComponent::removeAnimation(std::string const& name)
{
	mAnimations.erase(mAnimations.find(name));
}

void AnimatorComponent::removeAllAnimations()
{
	mAnimations.clear();
}

bool AnimatorComponent::isPlaying() const
{
	return mPlaying;
}

void AnimatorComponent::playAnimation(std::string const& name)
{
	mActualFrame = 0;
	mTimeElapsed = sf::Time::Zero;
	if (mAnimations.find(name) != mAnimations.end())
	{
		mActualAnimation = name;
		mActualTexture = getActualFrame().textureName;
		mSprite.setTexture(getApplication().getResource<Texture>(mActualTexture));
		mSprite.setTextureRect(getActualFrame().textureRect);
		mPlaying = true;
	}
	else
	{
		mActualAnimation = "";
	}
}

void AnimatorComponent::stopAnimation()
{
	mPlaying = false;
}

sf::Time AnimatorComponent::getElapsedTime() const
{
	return mTimeElapsed;
}

void AnimatorComponent::setElapsedTime(sf::Time elapsed)
{
	mTimeElapsed = elapsed;
	std::size_t size = getActualAnimation().getFrameCount();
	for (std::size_t i = 0; i < size; i++)
	{
		update(sf::Time::Zero);
	}
}

Animation& AnimatorComponent::getActualAnimation()
{
	return mAnimations[mActualAnimation];
}

Animation::Frame& AnimatorComponent::getActualFrame()
{
	return getActualAnimation().getFrame(mActualFrame);
}

void AnimatorComponent::update(sf::Time dt)
{
	if (mPlaying && mActualAnimation != "")
	{
		mTimeElapsed += dt;
		sf::Time t = getActualFrame().duration;
		if (mTimeElapsed >= t)
		{
			mActualFrame = (mActualFrame + 1) % getActualAnimation().getFrameCount();
			const Animation::Frame& newFrame = getActualFrame();
			if (mActualTexture != newFrame.textureName)
			{
				mActualTexture = newFrame.textureName;
				mSprite.setTexture(getApplication().getResource<Texture>(mActualTexture));
			}
			mSprite.setTextureRect(newFrame.textureRect);
			mTimeElapsed -= t;
		}
	}
}

sf::FloatRect AnimatorComponent::getLocalBounds()
{
	return getTransform().transformRect(mSprite.getLocalBounds());
}

sf::FloatRect AnimatorComponent::getGlobalBounds()
{
	return getWorldTransform().transformRect(mSprite.getLocalBounds());
}

void AnimatorComponent::setColor(const sf::Color& color)
{
	mSprite.setColor(color);
}

const sf::Color& AnimatorComponent::getColor() const
{
	return mSprite.getColor();
}

void AnimatorComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);

	serializer.save("current", mActualAnimation);
	serializer.save("playing", isPlaying());
	serializer.save("elapsed", getElapsedTime());

	for (auto itr = mAnimations.begin(); itr != mAnimations.end(); itr++)
	{
		serializer.create("Animation");
		serializer.save("name", itr->first);
		serializer.save("animation", itr->second.getName());
		serializer.close();
	}
}

bool AnimatorComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}

	if (!serializer.load("current", mActualAnimation))
	{
		getLog() << ke::Log::Warning << ke::Variant("AnimatorComponent::deserialize : Can't find \"current\" in ", getId());
		mActualAnimation = "";
	}

	if (!serializer.load("playing", mPlaying))
	{
		getLog() << ke::Log::Warning << ke::Variant("AnimatorComponent::deserialize : Can't find \"playing\" in ", getId());
		mPlaying = false;
	}

	sf::Time elapsed;
	if (!serializer.load("elapsed", elapsed))
	{
		getLog() << ke::Log::Warning << ke::Variant("AnimatorComponent::deserialize : Can't find \"elapsed\" in ", getId());
		elapsed = sf::Time::Zero;
	}

	for (pugi::xml_node animation : serializer.getNode().children("Animation"))
	{
		std::string name = animation.attribute("name").as_string();
		std::string aName = animation.attribute("animation").as_string();
		mAnimations[name] = getApplication().getResource<Animation>(aName);
	}

	if (mPlaying)
	{
		playAnimation(mActualAnimation);
	}
	setElapsedTime(elapsed);

	return true;
}

void AnimatorComponent::renderCurrent(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mSprite, states);
}

} // namespace ke