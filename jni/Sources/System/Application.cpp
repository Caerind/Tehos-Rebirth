#include "Application.hpp"

namespace ke
{

void Application::init()
{
	if (!instance().mOpen)
	{
		instance().mWindow.setErrorCallback(Log::error);
		instance().mWindow.setWarningCallback(Log::warning);
		instance().mWindow.setInfoCallback(Log::info);

		instance().mOpen = true;

		Log::info("Keengine::Application started");
		Log::info("Current time is " + DateTime().toString("%b %d, %Y %I:%M:%S %p"));
	}
}

void Application::quit()
{
	if (isAndroid())
	{
		std::exit(0);
	}
}

void Application::runState(std::string const& startState)
{
    pushState(startState);
    instance().mStates.update(sf::Time::Zero);
    instance().mStateMode = true;
    run();
}

void Application::setEventFunction(EventFunction event)
{
    instance().mEventFunction = event;
}

void Application::setUpdateFunction(UpdateFunction update)
{
    instance().mUpdateFunction = update;
}

void Application::setRenderFunction(RenderFunction render)
{
    instance().mRenderFunction = render;
}

void Application::runDefault()
{
    instance().mStateMode = false;
    run();
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timePerFrame = sf::seconds(1.f / 60.f);
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate >= timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            handleEvent();
            update(timePerFrame);
        }
        render();
    }
}

void Application::handleEvent()
{
    sf::Event event;
    while (instance().mWindow.handleEvent(event))
    {
		instance().mInputs.handleEvent(event);

        if (instance().mStateMode)
        {
            if (stateCount() > 0)
            {
                instance().mStates.handleEvent(event);
            }
        }
        else if (instance().mEventFunction)
        {
            instance().mEventFunction(event);
        }

		// TODO : Move as Window callback
		if (event.type == sf::Event::LostFocus && isAndroid())
		{
			instance().mAudio.pause();
		}
		if (event.type == sf::Event::GainedFocus && isAndroid())
		{
			instance().mAudio.play();
		}
    }
}

void Application::update(sf::Time dt)
{
	instance().mAudio.update();

	dt *= instance().mTime.getTimeFactor();

	instance().mInputs.update(dt);
	instance().mTime.update(dt);

    if (instance().mStateMode)
    {
        if (stateCount() > 0)
        {
            instance().mStates.update(dt);
        }
        if (stateCount() <= 0)
        {
            close();
        }
    }
    else if (instance().mUpdateFunction)
    {
        instance().mUpdateFunction(dt);
    }
}

void Application::render()
{
	instance().mWindow.clear();

    if (instance().mStateMode)
    {
        if (stateCount() > 0)
        {
            instance().mStates.render(instance().mWindow.getHandle());
        }
    }
    else if (instance().mRenderFunction)
    {
        instance().mRenderFunction(instance().mWindow.getHandle());
    }
	
    instance().mWindow.display();
}

Application& Application::instance()
{
    static Application* instance = new Application();
    return *instance;
}

Log& Application::getLog()
{
    return Log::instance();
}

ResourceManager& Application::getResources()
{
	return instance().mResources;
}

Window& Application::getWindow()
{
	return instance().mWindow;
}

PropertiesHolder& Application::getValues()
{
	return instance().mProperties;
}

TimeSystem& Application::getTime()
{
	return instance().mTime;
}

InputSystem& Application::getInputs()
{
	return instance().mInputs;
}

void Application::registerMusicFile(std::string const& id, std::string const& filename)
{
    instance().mAudio.registerMusicFile(id, filename);
}

std::shared_ptr<sf::Music> Application::playMusic(std::string const& id, bool loop, sf::Vector2f position)
{
    return instance().mAudio.playMusic(id, loop, position);
}

void Application::removeMusic(std::string const& id)
{
    instance().mAudio.removeMusic(id);
}

std::shared_ptr<sf::Sound> Application::playSound(std::string const& id, sf::Vector2f position)
{
    return instance().mAudio.playSound(id, position);
}

void Application::playAudio()
{
    instance().mAudio.play();
}

void Application::pauseAudio()
{
    instance().mAudio.pause();
}

void Application::stopAudio()
{
    instance().mAudio.stop();
}

void Application::setGlobalVolume(float volume)
{
    instance().mAudio.setGlobalVolume(volume);
}

void Application::setMusicVolume(float volume)
{
    instance().mAudio.setMusicVolume(volume);
}

void Application::setSoundVolume(float volume)
{
    instance().mAudio.setSoundVolume(volume);
}

float Application::getGlobalVolume()
{
    return instance().mAudio.getGlobalVolume();
}

float Application::getMusicVolume()
{
    return instance().mAudio.getMusicVolume();
}

float Application::getSoundVolume()
{
    return instance().mAudio.getSoundVolume();
}

sf::SoundSource::Status Application::getAudioStatus()
{
    return instance().mAudio.getStatus();
}

bool Application::hasResource(const std::string& name, const std::string& type)
{
	return instance().mResources.hasResource(name, type);
}

bool Application::isResourceLoaded(const std::string& name, const std::string& type)
{
    return instance().mResources.isResourceLoaded(name, type);
}

void Application::releaseResource(const std::string& name, const std::string& type)
{
    instance().mResources.releaseResource(name, type);
}

void Application::releaseAllResources(const std::string& type)
{
    instance().mResources.releaseAllResources(type);
}

std::size_t Application::countResources(const std::string& type)
{
	return instance().mResources.countResources(type);
}

bool Application::loadResourcesFromXml(const std::string& filename)
{
	return instance().mResources.loadFromXml(filename);
}

bool Application::loadResourcesFromMemory(const std::string& memory)
{
	return instance().mResources.loadFromMemory(memory);
}

bool Application::saveResourcesToXml(const std::string& filename)
{
	return instance().mResources.saveToXml(filename);
}

void Application::setAssetsPath(const std::string& path)
{
	instance().mAssetsPath = path;
}

const std::string& Application::getAssetsPath()
{
	return instance().mAssetsPath;
}

void Application::pushState(std::string const& id)
{
    instance().mStates.pushState(id);
}

void Application::popState()
{
    instance().mStates.popState();
}

void Application::clearStates()
{
    instance().mStates.clearStates();
}

std::size_t Application::stateCount()
{
    return instance().mStates.stateCount();
}

void Application::setName(const std::string& name)
{
	instance().mName = name;
}

const std::string& Application::getName()
{
	return instance().mName;
}

bool Application::isOpen()
{
	return instance().mOpen;
}

void Application::close()
{
	if (instance().mWindow.isOpen())
	{
		instance().mWindow.close();
	}
	instance().mOpen = false;
}

Application::Application()
{
    mEventFunction = [](sf::Event const& event){};
    mUpdateFunction = [](sf::Time dt){};
    mRenderFunction = [](sf::RenderTarget& target){};

	// TODO : Move Resources registration from here
	SfmlResources::registerResources(mResources);
	Animation::registerType(mResources);
	Configuration::registerType(mResources);
	Theme::registerType(mResources);
	Tileset::registerType(mResources);
}

Application::~Application()
{
    mResources.releaseAllResources();
}

} // namespace ke