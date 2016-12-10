#include "Sources/Config.hpp"
#include "Sources/System/Application.hpp"
#include "Sources/System/Joystick.hpp"
#include "Sources/Core/Scene.hpp"

#include "MyActor.hpp"
#include "MyObject.hpp"

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

	const std::string resources = "" \
	"<?xml version=\"1.0\"?>" \
	"<Resources>" \
	"<Texture name=\"background\" filename=\"" + ke::Application::getAssetsPath() + "background.png\" />" \
	"<Texture name=\"cat\" filename=\"" + ke::Application::getAssetsPath() + "cat.png\" />" \
	"<Texture name=\"joyBackground\" filename=\"" + ke::Application::getAssetsPath() + "joyBackground.png\" />" \
	"<Texture name=\"joyButton\" filename=\"" + ke::Application::getAssetsPath() + "joyButton.png\" />" \
	"<Animation name=\"ninja-cat-idle\">" \
	"<Frame textureName=\"cat\" textureRect=\"0, 0, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"64, 0, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"128, 0, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"192, 0, 64, 64\" duration=\"0.15\" />" \
	"</Animation>" \
	"<Animation name=\"ninja-cat-run\">" \
	"<Frame textureName=\"cat\" textureRect=\"0, 64, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"64, 64, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"128, 64, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"192, 64, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"256, 64, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"320, 64, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"384, 64, 64, 64\" duration=\"0.15\" />" \
	"<Frame textureName=\"cat\" textureRect=\"448, 64, 64, 64\" duration=\"0.15\" />" \
	"</Animation>" \
	"</Resources>";

	ke::Application::loadResourcesFromMemory(resources);
		
	ke::Window& window = ke::Application::getWindow();
	window.create(sf::VideoMode(1024, 768), ke::Application::getName(), sf::Style::Close);
	window.useBackgroundScaled(&ke::Application::getResource<ke::Texture>("background"));
	window.setAction(ke::Window::Console, sf::Keyboard::Quote);
	window.setAction(ke::Window::DebugInfo, sf::Keyboard::Quote);
	window.setAction(ke::Window::Screenshot, sf::Keyboard::F3);

	sf::View defaultView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

	sf::Vector2f ratio;
	ratio.x = static_cast<float>(window.getDefaultView().getSize().x) / defaultView.getSize().x;
	ratio.y = static_cast<float>(window.getDefaultView().getSize().y) / defaultView.getSize().y;

	if (ke::isWindows())
	{
		ke::Application::getInputs().setKeyboardMapping("MoveUp", sf::Keyboard::Z, ke::InputType::Pressed);
		ke::Application::getInputs().setKeyboardMapping("MoveLeft", sf::Keyboard::Q, ke::InputType::Hold);
		ke::Application::getInputs().setKeyboardMapping("MoveRight", sf::Keyboard::D, ke::InputType::Hold);
		ke::Application::getInputs().setKeyboardMapping("Light", sf::Keyboard::L, ke::InputType::Pressed);
	}

	ke::Joystick joystick;
	joystick.setButtonTexture(ke::Application::getResource<ke::Texture>("joyButton"));
	joystick.setBackgroundTexture(ke::Application::getResource<ke::Texture>("joyBackground"));
	joystick.setPosition(sf::Vector2f(800.f, 600.f) - sf::Vector2f(joystick.getBounds().width, joystick.getBounds().height));
	joystick.setDeltaMax(20.f);
	joystick.blockVertical(true);

	tgui::Gui gui(window.getHandle());
	ke::Theme& theme = ke::Application::getResource<ke::Theme>("css", ke::Application::getAssetsPath() + "widgets.css");
	tgui::Button::Ptr button = theme.create("Button");
	button->setText("Jump");
	button->setPosition(sf::Vector2f(100.f * ratio.x, 100.f * ratio.y));
	button->setSize(sf::Vector2f(200.f * ratio.x, 200.f * ratio.y));
	gui.add(button);

	ke::Scene scene(ke::Scene::Physic);
	scene.getPhysic()->setGravity();
	scene.getPhysic()->setPixelsPerMeter(32.f);
	scene.getPhysic()->setRenderDebug(true);
	scene.getView() = defaultView;

	MyActor::Ptr actor = scene.createActor<MyActor>("actor");
	actor->setPosition(100.f, 20.f);

	MyObject::Ptr ground = scene.createActor<MyObject>("ground");
	ground->setPosition(0.f, 500.f);
	ground->setSize(800, 600);

	button->connect("pressed", [&]()
	{
		actor->desiredImpulseY(-240.f);
	});

	ke::Application::setEventFunction([&](const sf::Event& event)
	{
		if (event.type == sf::Event::TouchBegan && event.touch.finger == 2)
		{
			ke::Application::close();
		}

		joystick.handleEvent(event, window.getPointerPositionView(defaultView));
		gui.handleEvent(event);
	});
	ke::Application::setUpdateFunction([&](sf::Time dt)
	{
		if (joystick.getDelta().x != 0.f)
		{
			actor->setVelocityX(joystick.getDelta().x * 8);
		}
		scene.update(dt);
		gui.updateTime(dt);
	});
	ke::Application::setRenderFunction([&](sf::RenderTarget& target)
	{
		scene.render(target);
		target.setView(defaultView);
		joystick.render(target);
		gui.draw();
	});
	ke::Application::runDefault();

	actor = nullptr;
	ground = nullptr;

	ke::Application::quit();

	return 0;
}