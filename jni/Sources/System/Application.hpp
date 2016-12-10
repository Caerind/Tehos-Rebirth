#ifndef KE_APPLICATION_HPP
#define KE_APPLICATION_HPP

#include "AudioManager.hpp"
#include "ResourceManager.hpp"
#include "StateManager.hpp"
#include "PropertiesHolder.hpp"
#include "Window.hpp"
#include "TimeSystem.hpp"
#include "InputSystem.hpp"

#include "../Config.hpp"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "Log.hpp"
#include "DateTime.hpp"
#include "../Maths/Math.hpp"

// TODO : Move Resources inclusions from here
#include "SfmlResources.hpp"
#include "Animation.hpp"
#include "Configuration.hpp"
#include "Theme.hpp"
#include "Tileset.hpp"

namespace ke
{

class Application
{
    public:
        using EventFunction = std::function<void(sf::Event const& event)> ;
        using UpdateFunction = std::function<void(sf::Time dt)>;
        using RenderFunction = std::function<void(sf::RenderTarget& target)>;

        static void init();
        static void quit();

        static void runState(std::string const& startState);

        static void setEventFunction(EventFunction event = [](sf::Event const& event){});
        static void setUpdateFunction(UpdateFunction update = [](sf::Time dt){});
        static void setRenderFunction(RenderFunction render = [](sf::RenderTarget& target){});
        static void runDefault();

        static void run();
        static void handleEvent();
        static void update(sf::Time dt);
        static void render();

        static Application& instance();
        static Log& getLog();
		static ResourceManager& getResources();
		static Window& getWindow();
		static PropertiesHolder& getValues();
		static TimeSystem& getTime();
		static InputSystem& getInputs();

        //
        // Audio
        //
        static void registerMusicFile(std::string const& id, std::string const& filename);
        static std::shared_ptr<sf::Music> playMusic(std::string const& id, bool loop = true, sf::Vector2f position = sf::Vector2f());
        static void removeMusic(std::string const& id);
        static std::shared_ptr<sf::Sound> playSound(std::string const& id, sf::Vector2f position = sf::Vector2f());
        static void playAudio();
        static void pauseAudio();
        static void stopAudio();
        static void setGlobalVolume(float volume);
        static void setMusicVolume(float volume);
        static void setSoundVolume(float volume);
        static float getGlobalVolume();
        static float getMusicVolume();
        static float getSoundVolume();
        static sf::SoundSource::Status getAudioStatus();

        //
        // Resources
        //
        template <typename T, typename ... Args>
        static T& getResource(const std::string& name, Args&& ... args);
		static bool hasResource(const std::string& name, const std::string& type = "");
		static bool isResourceLoaded(const std::string& name, const std::string& type = "");
		static void releaseResource(const std::string& name, const std::string& type = "");
		static void releaseAllResources(const std::string& type = "");
		static std::size_t countResources(const std::string& type = "");
		static bool loadResourcesFromXml(const std::string& filename);
		static bool loadResourcesFromMemory(const std::string& memory);
		static bool saveResourcesToXml(const std::string& filename);
		static void setAssetsPath(const std::string& path);
		static const std::string& getAssetsPath();
		// TODO : Improve Assets Path
		// TODO : Improve Resources registration

        //
        // States
        //
        template <typename T>
        static void registerState(std::string const& className);
        static void pushState(std::string const& id);
        static void popState();
        static void clearStates();
        static std::size_t stateCount();
		template <typename T>
		static std::shared_ptr<T> getActualState();

		//
		// Lang
		//
		// TODO : Lang handling

        //
        // Window
        //
		// TODO : Window functions

		//
		// Application Data
		//
		static void setName(const std::string& name);
		static const std::string& getName();
		static bool isOpen();
		static void close();

    private:
        Application();
        ~Application();

        AudioManager mAudio;
        ResourceManager mResources;
        StateManager mStates;
        Window mWindow;
		PropertiesHolder mProperties;
		TimeSystem mTime;
		InputSystem mInputs;

		std::string mAssetsPath;
        bool mStateMode; // Default Mode Or State Mode
		bool mOpen;
		std::string mName;

        EventFunction mEventFunction;
        UpdateFunction mUpdateFunction;
        RenderFunction mRenderFunction;
};

template <typename T, typename ... Args>
T& Application::getResource(const std::string& name, Args&& ... args)
{
    return instance().mResources.getResource<T>(name, std::forward<Args>(args)...);
}

template <typename T>
std::shared_ptr<T> Application::getActualState()
{
	return instance().mStates.getActualTypedState<T>();
}

template <typename T>
void Application::registerState(std::string const& className)
{
	instance().mStates.registerState<T>(className);
}

} // namespace ke

#endif // KE_APPLICATION_HPP
