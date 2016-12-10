#include "CameraComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

CameraComponent::CameraComponent(Actor& actor) 
	: SceneComponent(actor)
{
}

CameraComponent::~CameraComponent()
{
	onUnregister();
}

sf::View& CameraComponent::getView()
{
	return getScene().getView();
}

void CameraComponent::onTransformNotified()
{
	getView().setCenter(getWorldPosition());
	getView().setRotation(getRotation());
}

} // namespace ke
