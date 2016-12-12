#include "IntroState.hpp"
#include "../Sources/System/Application.hpp"
#include "GameState.hpp"

IntroState::IntroState() 
	: ke::State()
	, mElapsed(sf::Time::Zero)
{
	getApplication().getWindow().useBackgroundScaled(getApplication().getAssetsPath() + "background.png");
}

bool IntroState::update(sf::Time dt)
{
	mElapsed += dt;
	if (mElapsed > sf::seconds(1.0f))
	{
		loadResources();

		clearStates();
		// TODO : TEMP GAME FROM INTRO
		//pushState("MenuState");
		pushState("GameState");

		getApplication().playMusic("music");
	}
    return true;
}

void IntroState::loadResources()
{
	// Fonts
	ke::Application::getResource<ke::Font>("font", ke::Application::getAssetsPath() + "fonts/font.ttf");

	// Textures
	ke::Application::getResource<ke::Texture>("gui-game", ke::Application::getAssetsPath() + "gui-game.png");
	ke::Application::getResource<ke::Texture>("gui-end", ke::Application::getAssetsPath() + "gui-end.png");
	ke::Application::getResource<ke::Texture>("gui-settings", ke::Application::getAssetsPath() + "gui-settings.png");
	ke::Application::getResource<ke::Texture>("gui-pregame", ke::Application::getAssetsPath() + "gui-pregame.png");
	ke::Application::getResource<ke::Texture>("hero", ke::Application::getAssetsPath() + "hero.png");
	ke::Application::getResource<ke::Texture>("enemy-0", ke::Application::getAssetsPath() + "enemy-0.png");
	ke::Application::getResource<ke::Texture>("enemy-1", ke::Application::getAssetsPath() + "enemy-1.png");
	ke::Application::getResource<ke::Texture>("enemy-2", ke::Application::getAssetsPath() + "enemy-2.png");
	ke::Application::getResource<ke::Texture>("soldier-0", ke::Application::getAssetsPath() + "soldier-0.png");
	ke::Application::getResource<ke::Texture>("soldier-1", ke::Application::getAssetsPath() + "soldier-1.png");
	ke::Application::getResource<ke::Texture>("soldier-2", ke::Application::getAssetsPath() + "soldier-2.png");
	ke::Application::getResource<ke::Texture>("fx", ke::Application::getAssetsPath() + "fx.png");
	ke::Application::getResource<ke::Texture>("groundShadow", ke::Application::getAssetsPath() + "groundShadow.png");
	ke::Application::getResource<ke::Texture>("lifebar", ke::Application::getAssetsPath() + "lifebar.png");

	// Sounds
	ke::Application::getResource<ke::SoundBuffer>("atk", ke::Application::getAssetsPath() + "sounds/atk.wav");
	ke::Application::getResource<ke::SoundBuffer>("crystal", ke::Application::getAssetsPath() + "sounds/crystal.wav");
	ke::Application::getResource<ke::SoundBuffer>("die", ke::Application::getAssetsPath() + "sounds/die.wav");
	ke::Application::getResource<ke::SoundBuffer>("select", ke::Application::getAssetsPath() + "sounds/select.wav");
	ke::Application::getResource<ke::SoundBuffer>("sp-1", ke::Application::getAssetsPath() + "sounds/sp-1.wav");
	ke::Application::getResource<ke::SoundBuffer>("sp-2", ke::Application::getAssetsPath() + "sounds/sp-2.wav");
	ke::Application::getResource<ke::SoundBuffer>("sp-3", ke::Application::getAssetsPath() + "sounds/sp-3.wav");
	ke::Application::getResource<ke::SoundBuffer>("sp-4", ke::Application::getAssetsPath() + "sounds/sp-4.wav");
	ke::Application::getResource<ke::SoundBuffer>("spawn-e", ke::Application::getAssetsPath() + "sounds/spawn-e.wav");
	ke::Application::getResource<ke::SoundBuffer>("spawn-s", ke::Application::getAssetsPath() + "sounds/spawn-s.wav");

	// Music
	ke::Application::registerMusicFile("music", ke::Application::getAssetsPath() + "sounds/music.ogg");

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
	generateAIAnimation("enemy-1");
	generateAIAnimation("enemy-2", 2.f);
	generateAIAnimation("soldier-0");
	generateAIAnimation("soldier-1");
	generateAIAnimation("soldier-2");
}

void IntroState::generateHeroAnimation(const std::string& animation)
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

void IntroState::generateAIAnimation(const std::string& animation, float factor)
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

