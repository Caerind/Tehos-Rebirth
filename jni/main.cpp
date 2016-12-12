#include "Sources/Config.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/System/Joystick.hpp"
#include "Sources/Core/Scene.hpp"

#include "States/GameState.hpp"
#include "States/IntroState.hpp"
#include "States/SettingsState.hpp"
#include "States/MenuState.hpp"
#include "States/PreGameState.hpp"

int main(int argc, char** argv)
{
	std::string name = "Tehos : Rebirth";

	ke::Log::useConsole(true);
	ke::Log::useOnline(true, "http://atmog.altervista.org/Log/index.php");
	ke::Log::setOnlineData("appversion", ke::getPlatformName() + "_" + ke::getVersionName());
	ke::Log::setOnlineData("appname", name);

	ke::Application::init();
	ke::Application::setName(name);
	if (ke::isWindows())
	{
		ke::Application::setAssetsPath("assets/");
	}
	else if (ke::isAndroid())
	{
		ke::Application::setAssetsPath("");
	}

	ke::Window& window = ke::Application::getWindow();
	// TODO : DEBUG
	window.create(sf::VideoMode(1280, 720), ke::Application::getName(), sf::Style::Close);
	//window.create(sf::VideoMode(1280, 720), ke::Application::getName(), sf::Style::Fullscreen);
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, 1280.f, 720.f)));
	if (ke::isWindows())
	{
		window.setIcon(ke::Application::getAssetsPath() + "icon.png");
		window.useCustomMouseCursor(ke::Application::getAssetsPath() + "cursor.png");
		window.setMouseCursorOrigin(sf::Vector2f(9.f, 0.f));
		window.setMouseCursorRotation(-20.f);
	}

	// Config file
	ke::Configuration& config = ke::Application::getResource<ke::Configuration>("gamedata");
	if (!config.loadFromFile(ke::Application::getDataPath() + "gamedata.pure"))
	{
		GameState::FirstGame = true;

		config.setProperty("game.level", 0);
		config.setProperty("game.played", 0);
		config.setProperty("game.crystals", 10);
		config.setProperty("game.music", true);
		config.setProperty("game.sound", true);

		config.setProperty("hero.spell-1", 3);
		config.setProperty("hero.spell-2", 4);
		config.setProperty("hero.life", 1000);
		config.setProperty("hero.skill-1", 1);
		config.setProperty("hero.skill-2", 1);
		config.setProperty("hero.skill-3", 1);
		config.setProperty("hero.skill-4", 1);

		config.setProperty("enemy-0.life", 100);
		config.setProperty("enemy-0.speed", 100.f);
		config.setProperty("enemy-0.damage", 20);

		config.setProperty("enemy-1.life", 100);
		config.setProperty("enemy-1.speed", 100.f);
		config.setProperty("enemy-1.damage", 20);

		config.setProperty("enemy-2.life", 600);
		config.setProperty("enemy-2.speed", 70.f);
		config.setProperty("enemy-2.damage", 40);

		config.setProperty("soldier-0.life", 150);
		config.setProperty("soldier-0.speed", 100.f);
		config.setProperty("soldier-0.damage", 20);
		config.setProperty("soldier-0.price", 100);

		config.setProperty("soldier-1.life", 250);
		config.setProperty("soldier-1.speed", 100.f);
		config.setProperty("soldier-1.damage", 30);
		config.setProperty("soldier-1.price", 200);

		config.setProperty("soldier-2.life", 350);
		config.setProperty("soldier-2.speed", 100.f);
		config.setProperty("soldier-2.damage", 40);
		config.setProperty("soldier-2.price", 300);

		// TODO : DEBUG
		//config.saveToFile();
	}

	ke::Application::setMusicVolume((config.getPropertyAs<bool>("game.music")) ? 40.f : 0.f);
	ke::Application::setSoundVolume((config.getPropertyAs<bool>("game.sound")) ? 50.f : 0.f);

	ke::Application::registerState<GameState>("GameState");
	ke::Application::registerState<IntroState>("IntroState");
	ke::Application::registerState<SettingsState>("SettingsState");
	ke::Application::registerState<MenuState>("MenuState");
	ke::Application::registerState<PreGameState>("PreGameState");

	ke::Application::runState("IntroState");

	ke::Application::quit();

	return 0;
}