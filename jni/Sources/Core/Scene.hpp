#ifndef KE_SCENE_HPP
#define KE_SCENE_HPP

#include <SFML/System/Time.hpp>

#include "../ExtLibs/LetThereBeLight.hpp"

#include "../System/Application.hpp"
#include "../System/Background.hpp"

#include "Actor.hpp"
#include "Effect.hpp"
#include "LightTextures.hpp"
#include "PhysicSystem.hpp"

namespace ke
{

class Scene
{
	public:
		typedef std::unique_ptr<Scene> Ptr;

		enum Options
		{
			None = 0,
			Physic = 1 << 0,
			Effect = 1 << 1,
			Light = 1 << 2,
		};

	public:
		// Ctor & Dtor
		Scene(sf::Uint32 options = Options::None);
		virtual ~Scene();

		// Usage
		void handleEvent(const sf::Event& event);
		void update(sf::Time dt);
		void render(sf::RenderTarget& target);

		// Actor management
		template <typename T, typename ... Args>
		std::shared_ptr<T> createActor(std::string const& actorId, Args&& ... args)
		{
			std::shared_ptr<T> actor = std::make_shared<T>(*this, std::forward<Args>(args)...);
			if (actorId == "")
			{
				actor->setId(ke::decToHex<std::size_t>(mActorIdCounter++));
			}
			else
			{
				actor->setId(actorId);
			}
			actor->initializePhysic();
			actor->initializeComponents();
			actor->initialize();
			mActors.push_back(actor);
			return actor;
		}
		Actor::Ptr createActorFromFactory(const std::string& type);
		Actor::Ptr getActor(std::string const& id) const;
		Actor::Ptr getActor(std::size_t index) const;
		void removeActor(std::string const& id);
		void removeActor(std::size_t index);
		bool hasActor(std::string const& id) const;
		template <typename T>
		std::shared_ptr<T> getActorT(std::string const& id) const
		{
			Actor::Ptr actor = getActor(id);
			if (actor == nullptr)
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(actor);
		}
		template <typename T>
		std::shared_ptr<T> getActorT(std::size_t index) const
		{
			Actor::Ptr actor = getActor(index);
			if (actor == nullptr)
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(actor);
		}
		std::size_t getActorCount() const;

		// Options
		bool usePhysic() const;
		bool useEffect() const;
		bool useLight() const;

		// Physic
		std::shared_ptr<PhysicSystem> getPhysic();
		b2World* getPhysicWorld();

		// Lights
		std::shared_ptr<ltbl::LightSystem> getLights();

		// Effects
		template <typename T>
		void setEffect(std::size_t const& order)
		{
			mEffects[order] = std::make_shared<T>();
		}
		void setEffect(std::size_t const& order, const std::string& effect);
		template <typename T>
		std::shared_ptr<T> getEffect(std::size_t const& order)
		{
			auto itr = mEffects.find(order);
			if (itr != mEffects.end())
			{
				return std::dynamic_pointer_cast<T>(itr->second);
			}
			return nullptr;
		}
		void removeEffect(std::size_t const& order);
		
		// SceneRoot
		template <typename T, typename ... Args>
		std::shared_ptr<T> createComponent(Args&& ... args)
		{
			if (mSceneRoot != nullptr)
			{
				return mSceneRoot->createComponent<T>(std::forward<Args>(args)...);
			}
			return nullptr;
		}
		void removeComponent(Component::Ptr component);
		void attachComponent(SceneComponent::Ptr component);
		void detachComponent(SceneComponent::Ptr component);

		// Background
		void useBackgroundColor(const sf::Color& color);
		void useBackgroundScaledTexture(const std::string& textureName, sf::IntRect rect = sf::IntRect());
		void useBackgroundRepeatedTexture(const std::string& textureName, sf::IntRect rect = sf::IntRect());

		// Getters
		sf::View& getView();
		Input& getInput();
		static Log& getLog();
		static Application& getApplication();

		// Serialization
		bool loadFromXml(const std::string& filename);
		void saveToXml(const std::string& filename);

	private:
		static bool sortActor(Actor::Ptr a, Actor::Ptr b);

		void initLights();

		void renderComplex(sf::RenderTarget& target);
		void renderSimple(sf::RenderTarget& target);

	protected:
		sf::Uint32 mOptions; ///< Options used by the scene

		std::size_t mActorIdCounter; ///< Actor Id Counter

		std::vector<Actor::Ptr> mActors; ///< Actors

		sf::View mView; ///< SceneView

		std::shared_ptr<PhysicSystem> mPhysic; ///< Physic of the scene, used if defined has an option

		std::shared_ptr<ltbl::LightSystem> mLights; ///< Lights of the scene, used if defined has an option

		std::unique_ptr<sf::RenderTexture> mSceneTexture; ///< Scene Texture, used if defined has an option

		std::map<std::size_t, std::shared_ptr<ke::Effect>> mEffects; ///< Effects, used if defined has an option

		Input mInput; ///< Input manager

		Actor::Ptr mSceneRoot; ///< Scene Root

		Background mBackground; ///< Background
};

} // namespace ke

#endif // KE_SCENE_HPP