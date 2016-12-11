#include "Sources/Config.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/System/Joystick.hpp"
#include "Sources/Core/Scene.hpp"

#include "States/GameState.hpp"

void generateHeroAnimation(const std::string& animation);
void generateAIAnimation(const std::string& animation);

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
		ke::Application::getResource<ke::Texture>("hero-0", ke::Application::getAssetsPath() + "hero-0.png");
		ke::Application::getResource<ke::Texture>("enemy-0", ke::Application::getAssetsPath() + "enemy-0.png");
		ke::Application::getResource<ke::Texture>("soldier-0", ke::Application::getAssetsPath() + "soldier-0.png");
		ke::Application::getResource<ke::Texture>("fx", ke::Application::getAssetsPath() + "fx.png");
		ke::Application::getResource<ke::Texture>("groundShadow", ke::Application::getAssetsPath() + "groundShadow.png");
		
		// Tileset
		ke::Tileset& tileset = ke::Application::getResource<ke::Tileset>("tileset");
		tileset.setFirstGid(1);
		tileset.setTileCount(50);
		tileset.setColumns(10);
		tileset.setImageSource(ke::Application::getAssetsPath() + "tileset.png");
		tileset.setTileSize(sf::Vector2i(32, 32));
		tileset.getTexture();

		// Animations Hero
		generateHeroAnimation("hero-0");

		// Animations AI
		generateAIAnimation("enemy-0");
		generateAIAnimation("soldier-0");
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
		config.setProperty("hero.spell.1", 1);
		config.setProperty("hero.spell.2", 2);
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
		config.setProperty("terrain-0.code", "eJztlLEOwCAIRE0Ng/7/B3dqYohyh1jr0IHBKD4PDmtKqf5xRFybYic/B/g6t10LWEf0l0Xan3skWH+dHz1n8a072Pqi87P9H+lE79L73vp7/YD6sGr+vHUoQb6n/9ZZxNdzzTA8PVr9/7Xsnu7o/LMake8ywR8xtC72r9lRf68XEV86eT29I/+hOTldP8q39DG+eMv/7LsYfsTfs/q/iBv3gw5X");
		config.setProperty("terrain-1.code", "eJztlrESwCAIQ7l28/8/2L3XYiCRMjhk0rsX1IDDzMZRC11FivJvR891Fd9jImL4LHvlRVU/ek8760c87Kjfu9/V/mj9kWwhfjPn/8bIZFXJz74T9vyr+WoPCj7TK5j8M+9elf+vDKAeus8fBd/zoJz/FXzUB9qju/6//tAEj2QOyQ==");
		config.saveToFile();
	}

	ke::Application::registerState<GameState>("GameState");

	ke::Application::runState("GameState");

	ke::Application::quit();

	return 0;
}

void generateHeroAnimation(const std::string& animation)
{
	// Idle
	ke::Animation& idle = ke::Application::getResource<ke::Animation>(animation + "-idle");
	idle.addFrame(animation, sf::IntRect(0, 0, 32, 50), sf::seconds(5.f));

	// Cast
	ke::Animation& cast = ke::Application::getResource<ke::Animation>(animation + "-cast");
	cast.addFrame(animation, sf::IntRect(32, 0, 32, 50), sf::seconds(0.2f));
	cast.addFrame(animation, sf::IntRect(64, 0, 32, 50), sf::seconds(0.2f));
	cast.addFrame(animation, sf::IntRect(96, 0, 32, 50), sf::seconds(0.2f));
	cast.addFrame(animation, sf::IntRect(128, 0, 32, 50), sf::seconds(0.2f));
}


void generateAIAnimation(const std::string& animation)
{
	// Idle
	ke::Animation& idleSO = ke::Application::getResource<ke::Animation>(animation + "-idle-so");
	idleSO.addFrame(animation, sf::IntRect(0, 0, 64, 64), sf::seconds(5.f));
	ke::Animation& idleSE = ke::Application::getResource<ke::Animation>(animation + "-idle-se");
	idleSE.addFrame(animation, sf::IntRect(0, 64, 64, 64), sf::seconds(5.f));
	ke::Animation& idleNE = ke::Application::getResource<ke::Animation>(animation + "-idle-ne");
	idleNE.addFrame(animation, sf::IntRect(0, 128, 64, 64), sf::seconds(5.f));
	ke::Animation& idleNO = ke::Application::getResource<ke::Animation>(animation + "-idle-no");
	idleNO.addFrame(animation, sf::IntRect(0, 192, 64, 64), sf::seconds(5.f));

	// Walk
	ke::Animation& walkSO = ke::Application::getResource<ke::Animation>(animation + "-walk-so");
	walkSO.addFrame(animation, sf::IntRect(0, 0, 64, 64), sf::seconds(0.2f));
	walkSO.addFrame(animation, sf::IntRect(64, 0, 64, 64), sf::seconds(0.2f));
	walkSO.addFrame(animation, sf::IntRect(128, 0, 64, 64), sf::seconds(0.2f));
	walkSO.addFrame(animation, sf::IntRect(192, 0, 64, 64), sf::seconds(0.2f));
	ke::Animation& walkSE = ke::Application::getResource<ke::Animation>(animation + "-walk-se");
	walkSE.addFrame(animation, sf::IntRect(0, 64, 64, 64), sf::seconds(0.2f));
	walkSE.addFrame(animation, sf::IntRect(64, 64, 64, 64), sf::seconds(0.2f));
	walkSE.addFrame(animation, sf::IntRect(128, 64, 64, 64), sf::seconds(0.2f));
	walkSE.addFrame(animation, sf::IntRect(192, 64, 64, 64), sf::seconds(0.2f));
	ke::Animation& walkNE = ke::Application::getResource<ke::Animation>(animation + "-walk-ne");
	walkNE.addFrame(animation, sf::IntRect(0, 128, 64, 64), sf::seconds(0.2f));
	walkNE.addFrame(animation, sf::IntRect(64, 128, 64, 64), sf::seconds(0.2f));
	walkNE.addFrame(animation, sf::IntRect(128, 128, 64, 64), sf::seconds(0.2f));
	walkNE.addFrame(animation, sf::IntRect(192, 128, 64, 64), sf::seconds(0.2f));
	ke::Animation& walkNO = ke::Application::getResource<ke::Animation>(animation + "-walk-no");
	walkNO.addFrame(animation, sf::IntRect(0, 192, 64, 64), sf::seconds(0.2f));
	walkNO.addFrame(animation, sf::IntRect(64, 192, 64, 64), sf::seconds(0.2f));
	walkNO.addFrame(animation, sf::IntRect(128, 192, 64, 64), sf::seconds(0.2f));
	walkNO.addFrame(animation, sf::IntRect(192, 192, 64, 64), sf::seconds(0.2f));

	// Atk
	ke::Animation& atkSO = ke::Application::getResource<ke::Animation>(animation + "-atk-so");
	atkSO.addFrame(animation, sf::IntRect(0, 0, 64, 64), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(256, 0, 64, 64), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(320, 0, 64, 64), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(384, 0, 64, 64), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(320, 0, 64, 64), sf::seconds(0.2f));
	ke::Animation& atkSE = ke::Application::getResource<ke::Animation>(animation + "-atk-se");
	atkSE.addFrame(animation, sf::IntRect(0, 64, 64, 64), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(256, 64, 64, 64), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(320, 64, 64, 64), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(384, 64, 64, 64), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(320, 64, 64, 64), sf::seconds(0.2f));
	ke::Animation& atkNE = ke::Application::getResource<ke::Animation>(animation + "-atk-ne");
	atkNE.addFrame(animation, sf::IntRect(0, 128, 64, 64), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(256, 128, 64, 64), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(320, 128, 64, 64), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(384, 128, 64, 64), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(320, 128, 64, 64), sf::seconds(0.2f));
	ke::Animation& atkNO = ke::Application::getResource<ke::Animation>(animation + "-atk-no");
	atkNO.addFrame(animation, sf::IntRect(0, 192, 64, 64), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(256, 192, 64, 64), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(320, 192, 64, 64), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(384, 192, 64, 64), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(320, 192, 64, 64), sf::seconds(0.2f));

	// Dead
	ke::Animation& dead = ke::Application::getResource<ke::Animation>(animation + "-dead");
	dead.addFrame(animation, sf::IntRect(0, 256, 64, 64), sf::seconds(0.3f));
	dead.addFrame(animation, sf::IntRect(64, 256, 64, 64), sf::seconds(0.3f));
	dead.addFrame(animation, sf::IntRect(128, 256, 64, 64), sf::seconds(3.f)); // Be sure to not overpass

	// Pop
	ke::Animation& pop = ke::Application::getResource<ke::Animation>(animation + "-pop");
	pop.addFrame(animation, sf::IntRect(192, 256, 64, 64), sf::seconds(0.25f));
	pop.addFrame(animation, sf::IntRect(256, 256, 64, 64), sf::seconds(0.25f));
	pop.addFrame(animation, sf::IntRect(320, 256, 64, 64), sf::seconds(0.25f));
	pop.addFrame(animation, sf::IntRect(384, 256, 64, 64), sf::seconds(2.f)); // Be sure to not overpass
}