#include "InputComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

InputComponent::InputComponent(Actor& actor) 
	: Component(actor)
{
}

InputComponent::~InputComponent()
{
	onUnregister();
}

void InputComponent::onRegister()
{
	if (!isRegistered())
	{
		getApplication().getInputs().registerInput(this);
		Component::onRegister();
	}
}

void InputComponent::onUnregister()
{
	if (isRegistered())
	{
		getApplication().getInputs().unregisterInput(this);
		Component::onUnregister();
	}
}

void InputComponent::serialize(Serializer& serializer)
{
	Component::serialize(serializer);
	serializer.save("priority", getPriority());
}

bool InputComponent::deserialize(Serializer& serializer)
{
	if (!Component::deserialize(serializer))
	{
		return false;
	}

	float priority;
	if (!serializer.load("priority", priority))
	{
		getLog() << ke::Log::Warning << ke::Variant("InputComponent::deserialize : Can't find \"priority\" in ", getId());
		priority = 0.f;
	}
	setPriority(priority);

	return true;
}

} // namespace ke