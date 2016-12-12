#include "Sources/Config.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/System/Joystick.hpp"
#include "Sources/Core/Scene.hpp"

#include "States/GameState.hpp"
#include "States/IntroState.hpp"
#include "States/SettingsState.hpp"

void generateHeroAnimation(const std::string& animation);
void generateAIAnimation(const std::string& animation, float factor = 1.f);

int main(int argc, char** argv)
{
	// TODO : DEBUG
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
		ke::Application::getResource<ke::Texture>("gui-end", ke::Application::getAssetsPath() + "gui-end.png");
		ke::Application::getResource<ke::Texture>("gui-settings", ke::Application::getAssetsPath() + "gui-settings.png");
		ke::Application::getResource<ke::Texture>("hero", ke::Application::getAssetsPath() + "hero.png");
		ke::Application::getResource<ke::Texture>("enemy-0", ke::Application::getAssetsPath() + "enemy-0.png");
		ke::Application::getResource<ke::Texture>("enemy-1", ke::Application::getAssetsPath() + "enemy-1.png");
		ke::Application::getResource<ke::Texture>("soldier-0", ke::Application::getAssetsPath() + "soldier-0.png");
		ke::Application::getResource<ke::Texture>("soldier-1", ke::Application::getAssetsPath() + "soldier-1.png");
		ke::Application::getResource<ke::Texture>("fx", ke::Application::getAssetsPath() + "fx.png");
		ke::Application::getResource<ke::Texture>("groundShadow", ke::Application::getAssetsPath() + "groundShadow.png");
		ke::Application::getResource<ke::Texture>("lifebar", ke::Application::getAssetsPath() + "lifebar.png");

		// Tileset
		ke::Tileset& tileset2 = ke::Application::getResource<ke::Tileset>("tileset");
		tileset2.setFirstGid(1);
		tileset2.setTileCount(20);
		tileset2.setColumns(10);
		tileset2.setImageSource(ke::Application::getAssetsPath() + "tileset.png");
		tileset2.setTileSize(sf::Vector2i(64, 64));
		tileset2.getTexture();

		// Animations Hero
		generateHeroAnimation("hero");

		// Animations AI
		generateAIAnimation("enemy-0");
		generateAIAnimation("enemy-1", 2.f);
		generateAIAnimation("soldier-0");
		generateAIAnimation("soldier-1");
	}

	ke::Window& window = ke::Application::getWindow();
	// TODO : DEBUG
	window.create(sf::VideoMode(1280, 720), ke::Application::getName(), sf::Style::Close);
	//window.create(sf::VideoMode(1280, 720), ke::Application::getName(), sf::Style::Fullscreen);
	if (ke::isWindows())
	{
		window.setIcon(ke::Application::getAssetsPath() + "icon.png");
		window.useCustomMouseCursor(ke::Application::getAssetsPath() + "cursor.png");

		// TODO : DEBUG
		window.setAction(ke::Window::Action::DebugInfo, sf::Keyboard::Quote);
		window.setAction(ke::Window::Action::Console, sf::Keyboard::Quote);
		window.setDebugInfoFont(&ke::Application::getResource<ke::Font>("font"));
		window.setConsoleFont(&ke::Application::getResource<ke::Font>("font"));
	}

	ke::Configuration& config = ke::Application::getResource<ke::Configuration>("gamedata");
	if (!config.loadFromFile(ke::Application::getDataPath() + "gamedata.pure"))
	{
		config.setProperty("game.level", 0);
		config.setProperty("game.played", 0);
		config.setProperty("game.crystals", 10);

		config.setProperty("hero.spell-1", 1);
		config.setProperty("hero.spell-2", 2);
		config.setProperty("hero.life", 1000);
		config.setProperty("hero.skill-1", 1);
		config.setProperty("hero.skill-2", 1);
		config.setProperty("hero.skill-3", 1);
		config.setProperty("hero.skill-4", 1);

		config.setProperty("enemy-0.life", 100);
		config.setProperty("enemy-0.speed", 100.f);
		config.setProperty("enemy-0.damage", 20);

		config.setProperty("enemy-1.life", 600);
		config.setProperty("enemy-1.speed", 70.f);
		config.setProperty("enemy-1.damage", 40);

		config.setProperty("soldier-0.life", 150);
		config.setProperty("soldier-0.speed", 100.f);
		config.setProperty("soldier-0.damage", 20);
		config.setProperty("soldier-0.price", 100);

		config.setProperty("soldier-1.life", 250);
		config.setProperty("soldier-1.speed", 100.f);
		config.setProperty("soldier-1.damage", 30);
		config.setProperty("soldier-1.price", 200);
		// TODO : DEBUG
		//config.saveToFile();
	}

	ke::Application::registerState<GameState>("GameState");
	ke::Application::registerState<IntroState>("IntroState");
	ke::Application::registerState<SettingsState>("SettingsState");

	ke::Application::runState("IntroState");

	ke::Application::quit();

	return 0;
}

void generateHeroAnimation(const std::string& animation)
{
	// Idle
	ke::Animation& idle = ke::Application::getResource<ke::Animation>(animation + "-idle");
	idle.addFrame(animation, sf::IntRect(128, 0, 128, 128), sf::seconds(5.f));

	// Cast
	ke::Animation& cast = ke::Application::getResource<ke::Animation>(animation + "-cast");
	cast.addFrame(animation, sf::IntRect(256, 0, 128, 128), sf::seconds(0.1f)); // 2
	cast.addFrame(animation, sf::IntRect(384, 0, 128, 128), sf::seconds(0.1f)); // 3
	cast.addFrame(animation, sf::IntRect(512, 0, 128, 128), sf::seconds(0.1f)); // 4
	cast.addFrame(animation, sf::IntRect(640, 0, 128, 128), sf::seconds(0.1f)); // 5
	cast.addFrame(animation, sf::IntRect(512, 0, 128, 128), sf::seconds(0.1f)); // 4
	cast.addFrame(animation, sf::IntRect(384, 0, 128, 128), sf::seconds(0.1f)); // 3
	cast.addFrame(animation, sf::IntRect(256, 0, 128, 128), sf::seconds(0.1f)); // 2
	cast.addFrame(animation, sf::IntRect(128, 0, 128, 128), sf::seconds(0.5f)); // 1 // Be sure to not overpass
}


void generateAIAnimation(const std::string& animation, float factor)
{
	// Idle
	int s = static_cast<int>(factor * 64.f);
	ke::Animation& idleSO = ke::Application::getResource<ke::Animation>(animation + "-idle-so");
	idleSO.addFrame(animation, sf::IntRect(0 * s, 0 * s, s, s), sf::seconds(5.f));
	ke::Animation& idleSE = ke::Application::getResource<ke::Animation>(animation + "-idle-se");
	idleSE.addFrame(animation, sf::IntRect(0 * s, 1 * s, s, s), sf::seconds(5.f));
	ke::Animation& idleNE = ke::Application::getResource<ke::Animation>(animation + "-idle-ne");
	idleNE.addFrame(animation, sf::IntRect(0 * s, 2 * s, s, s), sf::seconds(5.f));
	ke::Animation& idleNO = ke::Application::getResource<ke::Animation>(animation + "-idle-no");
	idleNO.addFrame(animation, sf::IntRect(0 * s, 3 * s, s, s), sf::seconds(5.f));

	// Walk
	ke::Animation& walkSO = ke::Application::getResource<ke::Animation>(animation + "-walk-so");
	walkSO.addFrame(animation, sf::IntRect(0 * s, 0 * s, s, s), sf::seconds(0.2f));
	walkSO.addFrame(animation, sf::IntRect(1 * s, 0 * s, s, s), sf::seconds(0.2f));
	walkSO.addFrame(animation, sf::IntRect(2 * s, 0 * s, s, s), sf::seconds(0.2f));
	walkSO.addFrame(animation, sf::IntRect(3 * s, 0 * s, s, s), sf::seconds(0.2f));
	ke::Animation& walkSE = ke::Application::getResource<ke::Animation>(animation + "-walk-se");
	walkSE.addFrame(animation, sf::IntRect(0 * s, 1 * s, s, s), sf::seconds(0.2f));
	walkSE.addFrame(animation, sf::IntRect(1 * s, 1 * s, s, s), sf::seconds(0.2f));
	walkSE.addFrame(animation, sf::IntRect(2 * s, 1 * s, s, s), sf::seconds(0.2f));
	walkSE.addFrame(animation, sf::IntRect(3 * s, 1 * s, s, s), sf::seconds(0.2f));
	ke::Animation& walkNE = ke::Application::getResource<ke::Animation>(animation + "-walk-ne");
	walkNE.addFrame(animation, sf::IntRect(0 * s, 2 * s, s, s), sf::seconds(0.2f));
	walkNE.addFrame(animation, sf::IntRect(1 * s, 2 * s, s, s), sf::seconds(0.2f));
	walkNE.addFrame(animation, sf::IntRect(2 * s, 2 * s, s, s), sf::seconds(0.2f));
	walkNE.addFrame(animation, sf::IntRect(3 * s, 2 * s, s, s), sf::seconds(0.2f));
	ke::Animation& walkNO = ke::Application::getResource<ke::Animation>(animation + "-walk-no");
	walkNO.addFrame(animation, sf::IntRect(0 * s, 3 * s, s, s), sf::seconds(0.2f));
	walkNO.addFrame(animation, sf::IntRect(1 * s, 3 * s, s, s), sf::seconds(0.2f));
	walkNO.addFrame(animation, sf::IntRect(2 * s, 3 * s, s, s), sf::seconds(0.2f));
	walkNO.addFrame(animation, sf::IntRect(3 * s, 3 * s, s, s), sf::seconds(0.2f));

	// Atk
	ke::Animation& atkSO = ke::Application::getResource<ke::Animation>(animation + "-atk-so");
	atkSO.addFrame(animation, sf::IntRect(0 * s, 0 * s, s, s), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(4 * s, 0 * s, s, s), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(5 * s, 0 * s, s, s), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(6 * s, 0 * s, s, s), sf::seconds(0.2f));
	atkSO.addFrame(animation, sf::IntRect(5 * s, 0 * s, s, s), sf::seconds(0.2f));
	ke::Animation& atkSE = ke::Application::getResource<ke::Animation>(animation + "-atk-se");
	atkSE.addFrame(animation, sf::IntRect(0 * s, 1 * s, s, s), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(4 * s, 1 * s, s, s), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(5 * s, 1 * s, s, s), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(6 * s, 1 * s, s, s), sf::seconds(0.2f));
	atkSE.addFrame(animation, sf::IntRect(5 * s, 1 * s, s, s), sf::seconds(0.2f));
	ke::Animation& atkNE = ke::Application::getResource<ke::Animation>(animation + "-atk-ne");
	atkNE.addFrame(animation, sf::IntRect(0 * s, 2 * s, s, s), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(4 * s, 2 * s, s, s), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(5 * s, 2 * s, s, s), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(6 * s, 2 * s, s, s), sf::seconds(0.2f));
	atkNE.addFrame(animation, sf::IntRect(5 * s, 2 * s, s, s), sf::seconds(0.2f));
	ke::Animation& atkNO = ke::Application::getResource<ke::Animation>(animation + "-atk-no");
	atkNO.addFrame(animation, sf::IntRect(0 * s, 3 * s, s, s), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(4 * s, 3 * s, s, s), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(5 * s, 3 * s, s, s), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(6 * s, 3 * s, s, s), sf::seconds(0.2f));
	atkNO.addFrame(animation, sf::IntRect(5 * s, 3 * s, s, s), sf::seconds(0.2f));

	// Dead
	ke::Animation& dead = ke::Application::getResource<ke::Animation>(animation + "-dead");
	dead.addFrame(animation, sf::IntRect(0 * s, 4 * s, s, s), sf::seconds(0.3f));
	dead.addFrame(animation, sf::IntRect(1 * s, 4 * s, s, s), sf::seconds(0.3f));
	dead.addFrame(animation, sf::IntRect(2 * s, 4 * s, s, s), sf::seconds(3.f)); // Be sure to not overpass

	// Pop
	ke::Animation& pop = ke::Application::getResource<ke::Animation>(animation + "-pop");
	pop.addFrame(animation, sf::IntRect(3 * s, 4 * s, s, s), sf::seconds(0.125f));
	pop.addFrame(animation, sf::IntRect(4 * s, 4 * s, s, s), sf::seconds(0.125f));
	pop.addFrame(animation, sf::IntRect(5 * s, 4 * s, s, s), sf::seconds(0.125f));
	pop.addFrame(animation, sf::IntRect(6 * s, 4 * s, s, s), sf::seconds(2.f)); // Be sure to not overpass
}