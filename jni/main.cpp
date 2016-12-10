#include "Sources/Config.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/System/Joystick.hpp"
#include "Sources/Core/Scene.hpp"

#include "States/GameState.hpp"

void generateAnimation(const std::string& animation)
{
	// Idle
	ke::Animation& idleEast = ke::Application::getResource<ke::Animation>(animation + "-idle-east");
	idleEast.addFrame(animation, sf::IntRect(0, 0, 64, 64), sf::seconds(5.f));
	ke::Animation& idleNorth = ke::Application::getResource<ke::Animation>(animation + "-idle-north");
	idleNorth.addFrame(animation, sf::IntRect(0, 64, 64, 64), sf::seconds(5.f));
	ke::Animation& idleWest = ke::Application::getResource<ke::Animation>(animation + "-idle-west");
	idleWest.addFrame(animation, sf::IntRect(0, 128, 64, 64), sf::seconds(5.f));
	ke::Animation& idleSouth = ke::Application::getResource<ke::Animation>(animation + "-idle-south");
	idleSouth.addFrame(animation, sf::IntRect(0, 192, 64, 64), sf::seconds(5.f));

	// Walk
	ke::Animation& walkEast = ke::Application::getResource<ke::Animation>(animation + "-walk-east");
	walkEast.addFrame(animation, sf::IntRect(0, 0, 64, 64), sf::seconds(0.2f));
	walkEast.addFrame(animation, sf::IntRect(64, 0, 64, 64), sf::seconds(0.2f));
	walkEast.addFrame(animation, sf::IntRect(128, 0, 64, 64), sf::seconds(0.2f));
	walkEast.addFrame(animation, sf::IntRect(192, 0, 64, 64), sf::seconds(0.2f));
	ke::Animation& walkNorth = ke::Application::getResource<ke::Animation>(animation + "-walk-north");
	walkNorth.addFrame(animation, sf::IntRect(0, 64, 64, 64), sf::seconds(0.2f));
	walkNorth.addFrame(animation, sf::IntRect(64, 64, 64, 64), sf::seconds(0.2f));
	walkNorth.addFrame(animation, sf::IntRect(128, 64, 64, 64), sf::seconds(0.2f));
	walkNorth.addFrame(animation, sf::IntRect(192, 64, 64, 64), sf::seconds(0.2f));
	ke::Animation& walkWest = ke::Application::getResource<ke::Animation>(animation + "-walk-west");
	walkWest.addFrame(animation, sf::IntRect(0, 128, 64, 64), sf::seconds(0.2f));
	walkWest.addFrame(animation, sf::IntRect(64, 128, 64, 64), sf::seconds(0.2f));
	walkWest.addFrame(animation, sf::IntRect(128, 128, 64, 64), sf::seconds(0.2f));
	walkWest.addFrame(animation, sf::IntRect(192, 128, 64, 64), sf::seconds(0.2f));
	ke::Animation& walkSouth = ke::Application::getResource<ke::Animation>(animation + "-walk-south");
	walkSouth.addFrame(animation, sf::IntRect(0, 192, 64, 64), sf::seconds(0.2f));
	walkSouth.addFrame(animation, sf::IntRect(64, 192, 64, 64), sf::seconds(0.2f));
	walkSouth.addFrame(animation, sf::IntRect(128, 192, 64, 64), sf::seconds(0.2f));
	walkSouth.addFrame(animation, sf::IntRect(192, 192, 64, 64), sf::seconds(0.2f));

	// Atk
	ke::Animation& atkEast = ke::Application::getResource<ke::Animation>(animation + "-atk-east");
	atkEast.addFrame(animation, sf::IntRect(0, 0, 64, 64), sf::seconds(0.2f));
	atkEast.addFrame(animation, sf::IntRect(256, 0, 64, 64), sf::seconds(0.2f));
	atkEast.addFrame(animation, sf::IntRect(320, 0, 64, 64), sf::seconds(0.2f));
	atkEast.addFrame(animation, sf::IntRect(256, 0, 64, 64), sf::seconds(0.2f));
	ke::Animation& atkNorth = ke::Application::getResource<ke::Animation>(animation + "-atk-north");
	atkNorth.addFrame(animation, sf::IntRect(0, 64, 64, 64), sf::seconds(0.2f));
	atkNorth.addFrame(animation, sf::IntRect(256, 64, 64, 64), sf::seconds(0.2f));
	atkNorth.addFrame(animation, sf::IntRect(320, 64, 64, 64), sf::seconds(0.2f));
	atkNorth.addFrame(animation, sf::IntRect(256, 64, 64, 64), sf::seconds(0.2f));
	ke::Animation& atkWest = ke::Application::getResource<ke::Animation>(animation + "-atk-west");
	atkWest.addFrame(animation, sf::IntRect(0, 128, 64, 64), sf::seconds(0.2f));
	atkWest.addFrame(animation, sf::IntRect(256, 128, 64, 64), sf::seconds(0.2f));
	atkWest.addFrame(animation, sf::IntRect(320, 128, 64, 64), sf::seconds(0.2f));
	atkWest.addFrame(animation, sf::IntRect(256, 128, 64, 64), sf::seconds(0.2f));
	ke::Animation& atkSouth = ke::Application::getResource<ke::Animation>(animation + "-atk-south");
	atkSouth.addFrame(animation, sf::IntRect(0, 192, 64, 64), sf::seconds(0.2f));
	atkSouth.addFrame(animation, sf::IntRect(256, 192, 64, 64), sf::seconds(0.2f));
	atkSouth.addFrame(animation, sf::IntRect(320, 192, 64, 64), sf::seconds(0.2f));
	atkSouth.addFrame(animation, sf::IntRect(256, 192, 64, 64), sf::seconds(0.2f));
}

int main(int argc, char** argv)
{
	std::string name = "LudumDare37";

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

	// Load resources
	{
		// Fonts
		ke::Application::getResource<ke::Font>("font", ke::Application::getAssetsPath() + "font.ttf");

		// Textures
		ke::Application::getResource<ke::Texture>("gui-game", ke::Application::getAssetsPath() + "gui-game.png");
		ke::Application::getResource<ke::Texture>("hero", ke::Application::getAssetsPath() + "hero.png");
		ke::Application::getResource<ke::Texture>("enemy-0", ke::Application::getAssetsPath() + "enemy-0.png");
		ke::Application::getResource<ke::Texture>("soldier-0", ke::Application::getAssetsPath() + "soldier-0.png");
		
		// Tileset
		ke::Tileset& tileset = ke::Application::getResource<ke::Tileset>("tileset");
		tileset.setFirstGid(1);
		tileset.setTileCount(50);
		tileset.setColumns(10);
		tileset.setImageSource(ke::Application::getAssetsPath() + "tileset.png");
		tileset.setTileSize(sf::Vector2i(32, 32));
		tileset.getTexture();

		// Animations
		ke::Animation& hero0Idle = ke::Application::getResource<ke::Animation>("hero-0-idle");
		hero0Idle.addFrame("hero", sf::IntRect(0, 0, 32, 50), sf::seconds(5.f));
		ke::Animation& hero0Cast = ke::Application::getResource<ke::Animation>("hero-0-cast");
		hero0Cast.addFrame("hero", sf::IntRect(32, 0, 32, 50), sf::seconds(0.2f));
		hero0Cast.addFrame("hero", sf::IntRect(64, 0, 32, 50), sf::seconds(0.2f));
		hero0Cast.addFrame("hero", sf::IntRect(96, 0, 32, 50), sf::seconds(0.2f));
		hero0Cast.addFrame("hero", sf::IntRect(128, 0, 32, 50), sf::seconds(0.2f));

		generateAnimation("enemy-0");
		generateAnimation("soldier-0");
	}

	ke::Window& window = ke::Application::getWindow();
	window.create(sf::VideoMode(1024, 768), ke::Application::getName(), sf::Style::Close);
	window.useBackgroundScaled(&ke::Application::getResource<ke::Texture>("background"));
	window.setAction(ke::Window::Console, sf::Keyboard::Quote);
	window.setAction(ke::Window::DebugInfo, sf::Keyboard::Quote);
	window.setAction(ke::Window::Screenshot, sf::Keyboard::F3);
	window.setConsoleFont(&ke::Application::getResource<ke::Font>("font"));
	window.setDebugInfoFont(&ke::Application::getResource<ke::Font>("font"));

	ke::Configuration& config = ke::Application::getResource<ke::Configuration>("gamedata");
	if (!config.loadFromFile(ke::Application::getDataPath() + "gamedata.pure"))
	{
		config.setProperty("gameplayed", 0);
		config.setProperty("gamewon", 0);
		config.setProperty("gamelost", 0);
		config.setProperty("hero-0.life", 1000);
		config.setProperty("hero-0.cooldown", 5.f);
		config.setProperty("enemy-0.life", 100);
		config.setProperty("enemy-0.cooldown", 1.f);
		config.setProperty("enemy-0.distance", 50.f);
		config.setProperty("enemy-0.speed", 100.f);
		config.setProperty("enemy-0.damage", 20);
		config.setProperty("soldier-0.life", 200);
		config.setProperty("soldier-0.cooldown", 1.f);
		config.setProperty("soldier-0.distance", 50.f);
		config.setProperty("soldier-0.speed", 100.f);
		config.setProperty("soldier-0.damage", 20);
		config.saveToFile();
	}

	ke::Application::registerState<GameState>("GameState");

	ke::Application::runState("GameState");

	ke::Application::quit();

	return 0;
}