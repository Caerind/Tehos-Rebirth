#pragma once

#include "Scene.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include "Effect.hpp"
#include "Component.hpp"
#include "../Components/Components.hpp"

namespace ke
{

class Factories
{
	public:
		template <typename T>
		static void registerActor()
		{
			mActorFactory[T::getStaticType()] = [](Scene& scene) { return Actor::Ptr(new T(scene)); };
		}

		template <typename T>
		static void registerComponent()
		{
			mComponentFactory[T::getStaticType()] = [](Actor& actor) { return Component::Ptr(new T(actor)); };
		}

		template <typename T>
		static void registerEffect()
		{
			mEffectFactory[T::getStaticType()] = []() { return std::shared_ptr<ke::Effect>(new T()); };
		}

		static bool registerAll();

		template <typename T>
		static Actor::Ptr createActor(Scene& scene)
		{
			return createActor(scene, T::getStaticType());
		}

		static Actor::Ptr createActor(Scene& scene, const std::string& type);

		template <typename T>
		static Component::Ptr createComponent(Actor& actor)
		{
			return createComponent(actor, T::getStaticType());
		}

		static Component::Ptr createComponent(Actor& actor, const std::string& type);

		template <typename T>
		static std::shared_ptr<ke::Effect> createEffect()
		{
			return createEffect(T::getStaticType());
		}

		static std::shared_ptr<ke::Effect> createEffect(const std::string& type);

	private:
		static std::map<std::string, std::function<Actor::Ptr(Scene& scene)>> mActorFactory;

		static std::map<std::string, std::function<Component::Ptr(Actor& actor)>> mComponentFactory;

		static std::map<std::string, std::function<std::shared_ptr<ke::Effect>()>> mEffectFactory;

		static bool mRegistered;
};

} // namespace ke
