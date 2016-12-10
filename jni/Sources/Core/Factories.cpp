#include "Factories.hpp"

namespace ke
{

std::map<std::string, std::function<Actor::Ptr(Scene& scene)>> Factories::mActorFactory;
std::map<std::string, std::function<Component::Ptr(Actor& actor)>> Factories::mComponentFactory;
std::map<std::string, std::function<std::shared_ptr<ke::Effect>()>> Factories::mEffectFactory;
bool Factories::mRegistered = registerAll();

bool Factories::registerAll()
{
	if (!mRegistered)
	{
		registerComponent<AnimatorComponent>();
		registerComponent<AudioListenerComponent>();
		registerComponent<CameraComponent>();
		registerComponent<DirectionLightComponent>();
		registerComponent<InputComponent>();
		registerComponent<LayerComponent>();
		registerComponent<LightShapeComponent>();
		registerComponent<NodeComponent>();
		registerComponent<ParticleComponent>();
		registerComponent<PhysicComponent>();
		registerComponent<PointLightComponent>();
		registerComponent<ShapeComponent>();
		registerComponent<SpriteComponent>();
		registerComponent<TextComponent>();
		
		registerEffect<Effect>();
		registerEffect<Blur>();
		registerEffect<Pixelate>();

		registerActor<Actor>();
		registerActor<Map>();

		mRegistered = true;
	}
	return true;
}

Actor::Ptr Factories::createActor(Scene& scene, const std::string& type)
{
	if (!mRegistered)
	{
		registerAll();
	}
	if (mActorFactory.find(type) == mActorFactory.end())
	{
		ke::Log::instance() << ke::Log::Error << ke::Variant("Factory::Actor not registered : ", type);
		return nullptr;
	}
	return mActorFactory[type](scene);
}

Component::Ptr Factories::createComponent(Actor& actor, const std::string& type)
{
	if (!mRegistered)
	{
		registerAll();
	}
	if (mComponentFactory.find(type) == mComponentFactory.end())
	{
		ke::Log::instance() << ke::Log::Error << ke::Variant("Factory::Component not registered : ", type);
		return nullptr;
	}
	return mComponentFactory[type](actor);
}

std::shared_ptr<ke::Effect> Factories::createEffect(const std::string& type)
{
	if (!mRegistered)
	{
		registerAll();
	}
	if (mEffectFactory.find(type) == mEffectFactory.end())
	{
		ke::Log::instance() << ke::Log::Error << ke::Variant("Factory::Effect not registered : ", type);
		return nullptr;
	}
	return mEffectFactory[type]();
}

} // namespace ke