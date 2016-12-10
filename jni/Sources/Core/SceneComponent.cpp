#include "SceneComponent.hpp"
#include "Actor.hpp"

namespace ke
{

SceneComponent::SceneComponent(Actor& actor)
	: Component(actor)
	, mTransformable()
	, mWorldTransform()
	, mNeedUpdateTransform(true)
	, mZ(0.f)
	, mParent(nullptr)
	, mChildren()
	, mVisible(true)
{
}

SceneComponent::~SceneComponent()
{
	onUnregister();
}

void SceneComponent::onRegister()
{
	Component::onRegister();
}

void SceneComponent::onUnregister()
{
	if (isRegistered())
	{
		if (mParent != nullptr)
		{
			mParent->detachComponent(this);
		}
		Component::onUnregister();
	}
}

const sf::Vector2f& SceneComponent::getPosition() const
{
	return mTransformable.getPosition();
}

void SceneComponent::setPosition(sf::Vector2f const& position)
{
	mTransformable.setPosition(position);
	onTransformChanged();
}

void SceneComponent::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

void SceneComponent::move(sf::Vector2f const& movement)
{
	mTransformable.move(movement);
	onTransformChanged();
}

void SceneComponent::move(float x, float y)
{
	move(sf::Vector2f(x, y));
}

float SceneComponent::getRotation() const
{
	return mTransformable.getRotation();
}

void SceneComponent::setRotation(float rotation)
{
	mTransformable.setRotation(rotation);
	onTransformChanged();
}

void SceneComponent::rotate(float rotation)
{
	mTransformable.rotate(rotation);
	onTransformChanged();
}

const sf::Vector2f& SceneComponent::getScale() const
{
	return mTransformable.getScale();
}

void SceneComponent::setScale(sf::Vector2f const& scale)
{
	mTransformable.setScale(scale);
	onTransformChanged();
}

void SceneComponent::setScale(float x, float y)
{
	setScale(sf::Vector2f(x, y));
}

void SceneComponent::scale(sf::Vector2f const& scale)
{
	mTransformable.setScale(scale);
	onTransformChanged();
}

void SceneComponent::scale(float x, float y)
{
	scale(sf::Vector2f(x, y));
}

const sf::Transform& SceneComponent::getTransform() const
{
	return mTransformable.getTransform();
}

float SceneComponent::getZ() const
{
	return mZ;
}

void SceneComponent::setZ(float z)
{
	mZ = z;
}

void SceneComponent::moveZ(float z)
{
	mZ += z;
}

sf::Vector2f SceneComponent::getWorldPosition()
{
	return getWorldTransform().transformPoint(sf::Vector2f());
}

sf::Transform SceneComponent::getWorldTransform()
{
	if (mNeedUpdateTransform)
	{
		mWorldTransform = (mParent != nullptr) ? mParent->getWorldTransform() : sf::Transform();
		mWorldTransform *= mTransformable.getTransform();
		mNeedUpdateTransform = false;
		onTransformUpdated();
	}
	return mWorldTransform;
}

bool SceneComponent::renderable() const
{
	return false;
}

void SceneComponent::setVisible(bool visible)
{
	mVisible = visible;
}

bool SceneComponent::isVisible() const
{
	return mVisible;
}

void SceneComponent::render(sf::RenderTarget& target, sf::RenderStates states)
{
	states.transform *= getTransform();
	if (renderable() && isVisible())
	{
		renderCurrent(target, states);
	}
	renderChildren(target, states);
}

void SceneComponent::attachComponent(SceneComponent* component)
{
	if (component != nullptr)
	{
		component->mParent = this;
		component->onTransformChanged();
		mChildren.push_back(component);
	}
}

void SceneComponent::detachComponent(SceneComponent* component)
{
	if (component != nullptr)
	{
		std::size_t size = mChildren.size();
		for (std::size_t i = 0; i < size; i++)
		{ 
			if (mChildren[i] == component)
			{
				component->mParent = nullptr;
				component->onTransformChanged();
				mChildren.erase(i + mChildren.begin());
				return;
			}
		}
	}
}

std::string SceneComponent::getParentId() const
{
	return (mParent != nullptr) ? mParent->getId() : "";
}

bool SceneComponent::attachToParent(const std::string& parentId)
{
	if (parentId != "")
	{
		SceneComponent::Ptr parent = mActor.getComponentT<SceneComponent>(parentId);
		if (parent != nullptr)
		{
			parent->attachComponent(this);
			return true;
		}
	}
	else
	{
		mActor.attachComponent(this);
		return true;
	}
	return false;
}

void SceneComponent::serialize(Serializer& serializer)
{
	Component::serialize(serializer);

	serializer.save("visible", isVisible());
	serializer.save("parent", getParentId());
	serializer.save("pos", getPosition());
	serializer.save("rot", getRotation());
	serializer.save("sca", getScale());
	serializer.save("z", getZ());
}

bool SceneComponent::deserialize(Serializer& serializer)
{
	if (!Component::deserialize(serializer))
	{
		return false;
	}

	if (!serializer.load("visible", mVisible))
	{
		getLog() << ke::Log::Warning << ke::Variant("SceneComponent::deserialize : Can't find \"visible\" in : ", getId());
		mVisible = true;
	}

	std::string parent;
	if (!serializer.load("parent", parent))
	{
		getLog() << ke::Log::Warning << ke::Variant("SceneComponent::deserialize : Can't find \"parent\" in : ", getId());
		parent = "";
	}
	attachToParent(parent);

	sf::Vector2f pos;
	if (!serializer.load("pos", pos))
	{
		getLog() << ke::Log::Warning << ke::Variant("SceneComponent::deserialize : Can't find \"pos\" in : ", getId());
		pos = sf::Vector2f();
	}
	setPosition(pos);

	float rot;
	if (!serializer.load("rot", rot))
	{
		getLog() << ke::Log::Warning << ke::Variant("SceneComponent::deserialize : Can't find \"rot\" in : ", getId());
		rot = 0.f;
	}
	setRotation(rot);

	sf::Vector2f sca;
	if (!serializer.load("sca", sca))
	{
		getLog() << ke::Log::Warning << ke::Variant("SceneComponent::deserialize : Can't find \"sca\" in : ", getId());
		sca = sf::Vector2f(1.f, 1.f);
	}
	setScale(sca);

	float z;
	if (!serializer.load("z", z))
	{
		getLog() << ke::Log::Warning << ke::Variant("SceneComponent::deserialize : Can't find \"z\" in : ", getId());
		z = 0.f;
	}
	setZ(z);

	return true;
}

void SceneComponent::renderCurrent(sf::RenderTarget& target, sf::RenderStates states)
{
}

void SceneComponent::renderChildren(sf::RenderTarget& target, sf::RenderStates states)
{
	std::sort(mChildren.begin(), mChildren.end(), sortChildren);

	std::size_t size = mChildren.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (mChildren[i] != nullptr)
		{
			mChildren[i]->render(target, states);
		}
	}
}

bool SceneComponent::sortChildren(SceneComponent * a, SceneComponent * b)
{
	if (a != nullptr && b != nullptr)
	{
		if (a->getZ() < b->getZ())
		{
			return true;
		}
		else if (a->getZ() > b->getZ())
		{
			return false;
		}
		return (a->getWorldPosition().y < b->getWorldPosition().y);
	}
	return (a != nullptr);
}

void SceneComponent::onTransformChanged()
{
	mNeedUpdateTransform = true;
	std::size_t size = mChildren.size();
	for (std::size_t i = 0; i < size; i++)
	{
		mChildren[i]->onTransformChanged();
	}
	onTransformNotified();
}

void SceneComponent::onTransformNotified()
{
}

void SceneComponent::onTransformUpdated()
{
}

} // namespace ke