#include "Window.hpp"

namespace ke
{

Window::Window() : mWindow(*this)
{
	// Init render window
	mTitle = "";
	mVisible = false;
	mVerticalSyncEnabled = false;
	mKeyRepeatEnabled = true;
	mJoystickThreshold = 0.1f;
	mMouseCursorGrabbed = false;
	mSettings = sf::ContextSettings();
	mFullscreen = false;
	mFullscreenVideoMode = sf::VideoMode();
	mNonFullscreenVideoMode = sf::VideoMode();
	mNonFullscreenStyle = sf::Style::Close;
	mIconPath = "";

	// Init mouse cursor
	mMouseCursor = MouseCursor::Default;
	mMouseCursorTexture = nullptr;
	mMouseCursorTextureCreated = false;
	mMouseCursorTextureData = "";

	// Init screenshot
	mScreenshotPath = "";

	// Init background
	mBackgroundUsage = BackgroundUsage::Color;
	mBackgroundColor = sf::Color(0, 0, 0, 255);
	mBackgroundTexture = nullptr;
	mBackgroundTextureCreated = false;
	mBackgroundTextureData = "";

	// Init console
	mConsoleVisible = false;
	mConsoleFont = nullptr;
	mConsoleFontCreated = false;
	mConsoleFontData = "";
	mConsoleBackground.setFillColor(sf::Color(32, 32, 32, 192));
	mConsoleBackground.setSize(sf::Vector2f(mConsoleBackground.getSize().x, 15.f + 10.f));
	mConsoleBackground.setOrigin(0.f, 15.f + 10.f);
	mConsoleText.setFillColor(sf::Color::White);
	mConsoleText.setCharacterSize(15);
	mConsoleText.setOrigin(0.f, 15.f);
	setConsoleCommand("help", [this](const CommandArgs& args)
	{
		for (std::size_t i = 0; i < mConsoleCommands.size(); i++)
		{
			addConsoleLine(mConsoleCommands[i].name + " : " + mConsoleCommands[i].help);
		}
	}, "Display informations about commands", "Usage : help");
	setConsoleCommand("man", [this](const CommandArgs& args)
	{
		if (args.argsCount() >= 1)
		{
			std::string cmd = args[0];
			for (std::size_t i = 0; i < mConsoleCommands.size(); i++)
			{
				if (mConsoleCommands[i].name == cmd)
				{
					addConsoleLine(cmd + "\n" + mConsoleCommands[i].help + "\n" + mConsoleCommands[i].man);
				}
			}
		}
	}, "Display the manual of a command", "Usage : man commandName");
	mConsoleDisplayBegin = 0;
	mConsoleDisplayLineHeight = 0.f;
	mConsoleDisplayBackground.setFillColor(sf::Color(32, 32, 32, 128));
	mConsoleDisplayText.setCharacterSize(15);
	mConsoleDisplayText.setOrigin(sf::Vector2f(-5.f, -5.f));
	addConsoleLine("Console launched, type \"help\" to display the list of commands");

	// Init debug info
	mDebugInfoVisible = false;
	mDebugInfoFont = nullptr;
	mDebugInfoFontCreated = false;
	mDebugInfoFontData = "";
	mDebugInfoText.setPosition(5.f, 5.f);
	mDebugInfoText.setFillColor(sf::Color::White);
	mDebugInfoText.setOutlineColor(sf::Color::Black);
	mDebugInfoText.setOutlineThickness(1.f);
	mDebugInfoText.setCharacterSize(12);

	// Init FPS Counter
	mFpsCounter = 0;
	mLastFpsCounter = 0;
	setDebugInfo("FPS", "0");
	setDebugInfo("Mouse", "0,0");

	// Init callback
	mResizeCallback = [](const sf::Vector2u& size) {};
	mErrorCallback = [](const std::string& message) { std::cout << "[ERROR] " << message << std::endl; };
	mWarningCallback = [](const std::string& message) { std::cout << "[WARNING] " << message << std::endl; };
	mInfoCallback = [](const std::string& message) { std::cout << "[INFO] " << message << std::endl; };
	mLostFocusCallback = [](){};
	mGainedFocusCallback = [](){};
	mCloseCallback = []() { return true; };

	// Init actions
	for (unsigned int i = 0; i < Action::Count; i++)
	{
		mActions[i] = sf::Keyboard::Unknown;
	}
}

Window::~Window()
{
}

void Window::create()
{
	if (isFullscreen())
	{
		create(mFullscreenVideoMode, mTitle, sf::Style::Fullscreen, mSettings);
	}
	else
	{
		create(mNonFullscreenVideoMode, mTitle, mNonFullscreenStyle, mSettings);
	}
}

void Window::create(sf::VideoMode mode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings)
{
	#ifdef SFML_SYSTEM_WINDOWS
		if ((style & sf::Style::Fullscreen) != 0)
		{
			mFullscreen = true;
			if (sf::VideoMode::getFullscreenModes().size() > 0)
			{
				mFullscreenVideoMode = sf::VideoMode::getFullscreenModes()[0];
			}
			else
			{
				if (mErrorCallback)
				{
					mErrorCallback("Window : No fullscreen mode available");
				}
				mFullscreenVideoMode = sf::VideoMode();
			}
		}
		else
		{
			mFullscreen = false;
			if (mode.isValid())
			{
				mNonFullscreenVideoMode = mode;
			}
			else
			{
				if (mErrorCallback)
				{
					mErrorCallback("Window : Invalid video mode");
				}
				mNonFullscreenVideoMode = sf::VideoMode();
			}
		}
		mTitle = title;
		if (!mFullscreen)
		{
			mNonFullscreenStyle = style;
		}
		mSettings = settings;
	#else
		mFullscreen = true;
		mFullscreenVideoMode = sf::VideoMode::getDesktopMode();
		mTitle = "";
		mSettings = settings;
	#endif

	if (mFullscreen)
	{
		mWindow.create(mFullscreenVideoMode, mTitle, sf::Style::Fullscreen, settings);
	}
	else
	{
		mWindow.create(mNonFullscreenVideoMode, mTitle, mNonFullscreenStyle, settings);
	}
	mWindow.init();
	onResize();
	if (mIcon.getSize().x > 0 && mIcon.getSize().y > 0)
	{
		mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());
	}
	mVisible = true;
}

void Window::toFullscreen()
{
	create(sf::VideoMode(), mTitle, sf::Style::Fullscreen, mSettings);
}

void Window::toNonFullscreen()
{
	create(mNonFullscreenVideoMode, mTitle, mNonFullscreenStyle, mSettings);
}

void Window::toggleFullscreen()
{
	if (mFullscreen)
	{
		toNonFullscreen();
	}
	else
	{
		toFullscreen();
	}
}

void Window::close()
{
	bool c = true;
	if (mCloseCallback)
	{
		c = mCloseCallback();
	}
	if (c)
	{
		mVisible = false;
		mWindow.close();
	}
}

bool Window::isFullscreen() const
{
	return mFullscreen;
}

const sf::VideoMode& Window::getVideoMode() const
{
	return (mFullscreen) ? mFullscreenVideoMode : mNonFullscreenVideoMode;
}

sf::Uint32 Window::getStyle() const
{
	return (mFullscreen) ? sf::Style::Fullscreen : mNonFullscreenStyle;
}

const std::string& Window::getTitle() const
{
	return mTitle;
}

bool Window::isVerticalSyncEnabled() const
{
	return mVerticalSyncEnabled;
}

bool Window::isVisible() const
{
	return mVisible;
}

bool Window::isKeyRepeatEnabled() const
{
	return mKeyRepeatEnabled;
}

float Window::getJoystickThreshold() const
{
	return mJoystickThreshold;
}

bool Window::isMouseCursorGrabbed() const
{
	return mMouseCursorGrabbed;
}

const sf::Image& Window::getIcon() const
{
	return mIcon;
}

const std::string& Window::getIconPath() const
{
	return mIconPath;
}

sf::Vector2i Window::getPosition() const
{
	return mWindow.getPosition();
}

const sf::ContextSettings& Window::getSettings() const
{
	return mSettings;
}

sf::Vector2u Window::getSize() const
{
	return mWindow.getSize();
}

sf::WindowHandle Window::getSystemHandle() const
{
	return mWindow.getSystemHandle();
}

bool Window::hasFocus() const
{
	return mWindow.hasFocus();
}

void Window::setVerticalSyncEnabled(bool enabled)
{
	mVerticalSyncEnabled = enabled;
	mWindow.setVerticalSyncEnabled(enabled);
}

void Window::setVisible(bool visible)
{
	mVisible = visible;
	mWindow.setVisible(visible);
}

void Window::setKeyRepeatEnabled(bool enabled)
{
	mKeyRepeatEnabled = enabled;
	mWindow.setKeyRepeatEnabled(enabled);
}

void Window::setJoystickThreshold(float threshold)
{
	mJoystickThreshold = threshold;
	mWindow.setJoystickThreshold(threshold);
}

void Window::setMouseCursorGrabbed(bool grabbed)
{
	mMouseCursorGrabbed = grabbed;
	mWindow.setMouseCursorGrabbed(grabbed);
}

void Window::setIcon(const std::string& icon)
{
	if (mIcon.loadFromFile(icon))
	{
		mIconPath = icon;
		if (mIcon.getSize().x > 0 && mIcon.getSize().y > 0)
		{
			mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());
		}
	}
	else
	{
		mIconPath = "";
		if (mWarningCallback)
		{
			mWarningCallback("Can't find icon : " + icon);
		}
	}
}

void Window::setIcon(const sf::Image& icon)
{
	mIcon = icon;
	if (mIcon.getSize().x > 0 && mIcon.getSize().y > 0)
	{
		mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());
	}
}

void Window::setPosition(const sf::Vector2i& position)
{
	mWindow.setPosition(position);
}

void Window::setSize(const sf::Vector2u& size)
{
	mWindow.setSize(size);
}

void Window::setActive(bool active)
{
	mWindow.setActive(active);
}

void Window::requestFocus()
{
	mWindow.requestFocus();
}

bool Window::isOpen() const
{
	return mWindow.isOpen();
}

bool Window::handleEvent(sf::Event& event)
{
	#ifdef SFML_SYSTEM_ANDROID
		if (!hasFocus())
		{
			return waitEvent(event);
		}
	#endif
	return pollEvent(event);
}

void Window::clear(const sf::Color& color)
{
	if (mBackgroundUsage == BackgroundUsage::Color)
	{
		mWindow.clear(mBackgroundColor);
	}
	else
	{
		mWindow.clear(color);

		sf::Vector2i size = static_cast<sf::Vector2i>(mWindow.getView().getSize()) + sf::Vector2i(1, 1);

		if (mBackgroundUsage == BackgroundUsage::Scaled)
		{
			sf::Transform transform;
			transform.scale(static_cast<float>(size.x) / mBackgroundSprite.getGlobalBounds().width, static_cast<float>(size.y) / mBackgroundSprite.getGlobalBounds().height);
			transform.translate(mWindow.getView().getCenter() - mWindow.getView().getSize() * 0.5f);
			
			mWindow.draw(mBackgroundSprite, transform);
		}
		else
		{
			sf::Transform transform;
			transform.translate(mWindow.getView().getCenter() - mWindow.getView().getSize() * 0.5f);

			float width = mBackgroundSprite.getGlobalBounds().width;
			float height = mBackgroundSprite.getGlobalBounds().height;

			int sx = (size.x / static_cast<int>(width)) + 1;
			int sy = (size.y / static_cast<int>(height)) + 1;

			for (int j = 0; j < sy; j++)
			{
				sf::RenderStates states;
				states.transform *= transform;
				states.transform.translate(0.f, j * height);
				for (int i = 0; i < sx; i++)
				{
					mWindow.draw(mBackgroundSprite, states);
					states.transform.translate(width, 0.f);
				}
			}
		}
	}
}

void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	mWindow.draw(drawable, states);
}

void Window::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	mWindow.draw(vertices, vertexCount, type, states);
}

void Window::display()
{
	std::ostringstream mouseOss;
	sf::Vector2i mousePos = static_cast<sf::Vector2i>(getMousePosition());
	mouseOss << mousePos.x << "," << mousePos.y;
	setDebugInfo("Mouse", mouseOss.str());

	if (isDebugInfoVisible() || mMouseCursor == MouseCursor::Custom || isConsoleVisible())
	{
		sf::View oldView = mWindow.getView();
		applyDefaultView();

		if (isDebugInfoVisible())
		{
			std::ostringstream oss;
			for (auto itr = mDebugInfo.begin(); itr != mDebugInfo.end(); itr++)
			{
				oss << itr->first << ": " << itr->second << std::endl;
			}
			mDebugInfoText.setString(oss.str());
			mWindow.draw(mDebugInfoText);
		}

		if (isConsoleVisible())
		{
			if (mConsoleDisplayBegin >= 0 && mConsoleDisplayBegin < mConsoleDisplayLines.size() && mConsoleDisplayLines.size() > 0)
			{
				mWindow.draw(mConsoleDisplayBackground);

				std::ostringstream oss;
				unsigned int lineDisplayed = static_cast<unsigned int>(mConsoleDisplayBackground.getSize().y / mConsoleDisplayLineHeight);
				for (std::size_t i = mConsoleDisplayBegin; i < mConsoleDisplayLines.size() && i < mConsoleDisplayBegin + lineDisplayed; i++)
				{
					oss << mConsoleDisplayLines[i] << std::endl;
				}
				mConsoleDisplayText.setString(oss.str());
				mConsoleDisplayText.setPosition(0.f, static_cast<float>(getSize().y) * 0.5f);
				mWindow.draw(mConsoleDisplayText);
			}
			else
			{
				mConsoleDisplayBegin = 0;
				mConsoleDisplayText.setString("");
			}

			if (mConsoleBlinkTime.getElapsedTime() > sf::seconds(1.f))
			{
				mConsoleBlinkTime.restart();
			}
			mConsoleText.setString("> " + mConsoleContentBefore + ((mConsoleBlinkTime.getElapsedTime() > sf::seconds(0.5f)) ? "|" : " ") + mConsoleContentAfter);
			mWindow.draw(mConsoleBackground);
			mWindow.draw(mConsoleText);
		}

		if (mMouseCursor == MouseCursor::Custom)
		{
			mMouseCursorSprite.setPosition(getMousePosition());
			mWindow.draw(mMouseCursorSprite);
		}

		mWindow.setView(oldView);
	}

	mFpsCounter++;
	if (mFpsClock.getElapsedTime() >= sf::seconds(1.f))
	{
		mLastFpsCounter = mFpsCounter;
		mFpsCounter = 0;
		mFpsClock.restart();

		std::ostringstream oss;
		oss << mLastFpsCounter;
		setDebugInfo("FPS", oss.str());
	}

	mWindow.display();
}

void Window::setView(const sf::View& view)
{
	mWindow.setView(view);
}

void Window::applyDefaultView()
{
	mWindow.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(mWindow.getSize().x), static_cast<float>(mWindow.getSize().y))));
}

const sf::View& Window::getView() const
{
	return mWindow.getView();
}

const sf::View& Window::getDefaultView() const
{
	return mWindow.getDefaultView();
}

sf::IntRect Window::getViewport(const sf::View& view) const
{
	return mWindow.getViewport(view);
}

void Window::mapPixelToCoords(const sf::Vector2i& point, const sf::View& view)
{
	if (view.getSize() == sf::Vector2f())
	{	
		mWindow.mapPixelToCoords(point);
	}
	else
	{
		mWindow.mapPixelToCoords(point, view);
	}
}

void Window::mapCoordsToPixel(const sf::Vector2f& point, const sf::View& view)
{
	if (view.getSize() == sf::Vector2f())
	{
		mWindow.mapCoordsToPixel(point);
	}
	else
	{
		mWindow.mapCoordsToPixel(point, view);
	}
}

void Window::setMousePosition(const sf::Vector2f& position)
{
	sf::Mouse::setPosition(static_cast<sf::Vector2i>(position), mWindow);
}

void Window::setMousePositionView(const sf::Vector2f& position, const sf::View& view)
{
	sf::Mouse::setPosition(mWindow.mapCoordsToPixel(position, view));
}

sf::Vector2f Window::getMousePosition() const
{
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow));
}

sf::Vector2f Window::getMousePositionView(const sf::View& view)
{
	return mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), view);
}

sf::Vector2f Window::getTouchPosition(unsigned int touchIndex) const
{
	return static_cast<sf::Vector2f>(sf::Touch::getPosition(touchIndex));
}

sf::Vector2f Window::getTouchPositionView(const sf::View& view, unsigned int touchIndex)
{
	return mWindow.mapPixelToCoords(sf::Touch::getPosition(touchIndex), view);
}

void Window::setPointerPosition(const sf::Vector2f& position)
{
	#ifndef SFML_SYSTEM_ANDROID
		setMousePosition(position);
	#endif
}

void Window::setPointerPositionView(const sf::Vector2f& position, const sf::View& view)
{
	#ifndef SFML_SYSTEM_ANDROID
		setMousePositionView(position, view);
	#endif
}

sf::Vector2f Window::getPointerPosition(unsigned int touchIndex) const
{
	#ifdef SFML_SYSTEM_ANDROID
		return getTouchPosition(touchIndex);
	#else
		return getMousePosition();
	#endif
}

sf::Vector2f Window::getPointerPositionView(const sf::View& view, unsigned int touchIndex)
{
	#ifdef SFML_SYSTEM_ANDROID
		return getTouchPositionView(view, touchIndex);
	#else
		return getMousePositionView(view);
	#endif
}

Window::MouseCursor Window::getMouseCursor() const
{
	return mMouseCursor;
}

void Window::setMouseCursor(MouseCursor mouseCursor)
{
	mMouseCursor = mouseCursor;
	mWindow.setMouseCursorVisible(mMouseCursor == MouseCursor::Default);
}

void Window::hideMouseCursor()
{
	setMouseCursor(MouseCursor::None);
}

void Window::useDefaultMouseCursor()
{
	setMouseCursor(MouseCursor::Default);
}

void Window::useCustomMouseCursor(const std::string& texture, const sf::IntRect& textureRect, const sf::Vector2f& origin)
{
	setMouseCursorTexture(texture);
	if (textureRect != sf::IntRect())
	{
		setMouseCursorTextureRect(textureRect);
	}
	setMouseCursorOrigin(origin);
}

void Window::useCustomMouseCursor(sf::Texture* texture, const std::string& textureData, const sf::IntRect& textureRect, const sf::Vector2f& origin)
{
	setMouseCursorTexture(texture, textureData);
	if (textureRect != sf::IntRect())
	{
		setMouseCursorTextureRect(textureRect);
	}
	setMouseCursorOrigin(origin);
}

sf::Texture* Window::getMouseCursorTexture() const
{
	return mMouseCursorTexture;
}

const std::string& Window::getMouseCursorTextureData() const
{
	return mMouseCursorTextureData;
}

const sf::IntRect& Window::getMouseCursorTextureRect() const
{
	return mMouseCursorSprite.getTextureRect();
}

const sf::Vector2f& Window::getMouseCursorOrigin() const
{
	return mMouseCursorSprite.getOrigin();
}

const sf::Vector2f& Window::getMouseCursorScale() const
{
	return mMouseCursorSprite.getScale();
}

float Window::getMouseCursorRotation() const
{
	return mMouseCursorSprite.getRotation();
}

const sf::Color& Window::getMouseCursorColor() const
{
	return mMouseCursorSprite.getColor();
}

void Window::setMouseCursorTexture(const std::string& texture)
{
	if (!mMouseCursorTextureCreated)
	{
		mMouseCursorTexture = new sf::Texture();
		mMouseCursorTextureCreated = true;
	}
	if (mMouseCursorTexture->loadFromFile(texture))
	{
		mMouseCursorTextureData = texture;
		setMouseCursor(MouseCursor::Custom);
		mMouseCursorSprite.setTexture(*mMouseCursorTexture);
	}
	else
	{
		mMouseCursorTextureData = "";
		setMouseCursor(MouseCursor::Default);
		if (mWarningCallback)
		{
			mWarningCallback("Can't find cursor : " + texture);
		}
	}
}

void Window::setMouseCursorTexture(sf::Texture* texture, const std::string& textureData)
{
	if (mMouseCursorTextureCreated)
	{
		delete mMouseCursorTexture;
		mMouseCursorTexture = nullptr;
		mMouseCursorTextureCreated = false;
	}
	mMouseCursorTexture = texture;
	mMouseCursorTextureData = textureData;
	mMouseCursorSprite.setTexture(*mMouseCursorTexture);
}

void Window::setMouseCursorTextureRect(const sf::IntRect& textureRect)
{
	mMouseCursorSprite.setTextureRect(textureRect);
}

void Window::setMouseCursorOrigin(const sf::Vector2f& origin)
{
	mMouseCursorSprite.setOrigin(origin);
}

void Window::setMouseCursorScale(const sf::Vector2f& scale)
{
	mMouseCursorSprite.setScale(scale);
}

void Window::setMouseCursorRotation(float rotation)
{
	mMouseCursorSprite.setRotation(rotation);
}

void Window::setMouseCursorColor(const sf::Color& color)
{
	mMouseCursorSprite.setColor(color);
}

void Window::screenshot()
{
	std::time_t t = std::time(nullptr);
	std::tm time;
	#ifdef _MSC_VER
		localtime_s(&time, &t);
	#else
		time = *localtime(&t);
	#endif
	mktime(&time);

	std::ostringstream oss;
	oss << mScreenshotPath << std::put_time(&time, "%Y-%m-%d_%H-%M-%S") << ".png";

	if (mInfoCallback)
	{
		mInfoCallback("Screenshot : " + oss.str());
	}

	sf::Texture texture;
	texture.create(getSize().x, getSize().y);
	texture.update(mWindow);
	texture.copyToImage().saveToFile(oss.str());
}

const std::string& Window::getScreenshotPath() const
{
	return mScreenshotPath;
}

void Window::setScreenshotPath(const std::string& screenshotPath)
{
	mScreenshotPath = screenshotPath;
}

Window::BackgroundUsage Window::getBackgroundUsage() const
{
	return mBackgroundUsage;
}

void Window::setBackgroundUsage(BackgroundUsage usage)
{
	mBackgroundUsage = usage;
}

void Window::useBackgroundColor(const sf::Color& color)
{
	mBackgroundColor = color;
	setBackgroundUsage(BackgroundUsage::Color);
}

void Window::useBackgroundScaled(const std::string& texture, const sf::IntRect& rect)
{
	if (!mBackgroundTextureCreated)
	{
		mBackgroundTexture = new sf::Texture();
		mBackgroundTextureCreated = true;
	}
	if (mBackgroundTexture->loadFromFile(texture))
	{
		mBackgroundTextureData = texture;
		setBackgroundUsage(BackgroundUsage::Scaled);
		mBackgroundSprite.setTexture(*mBackgroundTexture);
	}
	else
	{
		mBackgroundTextureData = "";
		setBackgroundUsage(BackgroundUsage::Color);
		if (mWarningCallback)
		{
			mWarningCallback("Can't find background : " + texture);
		}
	}
	if (rect != sf::IntRect())
	{
		mBackgroundSprite.setTextureRect(rect);
	}
}

void Window::useBackgroundScaled(sf::Texture* texture, const std::string& textureData, const sf::IntRect& rect)
{
	if (mBackgroundTextureCreated)
	{
		delete mBackgroundTexture;
		mBackgroundTexture = nullptr;
		mBackgroundTextureCreated = false;
	}
	mBackgroundTexture = texture;
	mBackgroundTextureData = textureData;
	setBackgroundUsage(BackgroundUsage::Scaled);
	mBackgroundSprite.setTexture(*mBackgroundTexture);
	if (rect != sf::IntRect())
	{
		mBackgroundSprite.setTextureRect(rect);
	}
}

void Window::useBackgroundRepeated(const std::string& texture, const sf::IntRect& rect)
{
	if (!mBackgroundTextureCreated)
	{
		mBackgroundTexture = new sf::Texture();
		mBackgroundTextureCreated = true;
	}
	if (mBackgroundTexture->loadFromFile(texture))
	{
		mBackgroundTextureData = texture;
		setBackgroundUsage(BackgroundUsage::Repeated);
		mBackgroundSprite.setTexture(*mBackgroundTexture);
	}
	else
	{
		mBackgroundTextureData = "";
		setBackgroundUsage(BackgroundUsage::Color);
		if (mWarningCallback)
		{
			mWarningCallback("Can't find background : " + texture);
		}
	}
	if (rect != sf::IntRect())
	{
		mBackgroundSprite.setTextureRect(rect);
	}
}

void Window::useBackgroundRepeated(sf::Texture* texture, const std::string& textureData, const sf::IntRect& rect)
{
	if (mBackgroundTextureCreated)
	{
		delete mBackgroundTexture;
		mBackgroundTexture = nullptr;
		mBackgroundTextureCreated = false;
	}
	mBackgroundTexture = texture;
	mBackgroundTextureData = textureData;
	setBackgroundUsage(BackgroundUsage::Repeated);
	mBackgroundSprite.setTexture(*mBackgroundTexture);
	if (rect != sf::IntRect())
	{
		mBackgroundSprite.setTextureRect(rect);
	}
}

const sf::Color& Window::getBackgroundColor() const
{
	return mBackgroundColor;
}

sf::Texture* Window::getBackgroundTexture() const
{
	return mBackgroundTexture;
}

const std::string& Window::getBackgroundTextureData() const
{
	return mBackgroundTextureData;
}

const sf::IntRect& Window::getBackgroundTextureRect() const
{
	return mBackgroundSprite.getTextureRect();
}

Window::CommandArgs::CommandArgs()
{
}

Window::CommandArgs::CommandArgs(const std::string& args)
{
	parse(args);
}

std::string Window::CommandArgs::operator[](std::size_t index) const
{
	if (index >= 0 && index < args.size())
	{
		return args.at(index);
	}
	return "";
}

std::size_t Window::CommandArgs::argsCount() const
{
	return args.size();
}

std::vector<std::string> Window::CommandArgs::operator[](const std::string& index) const
{
	auto itr = options.find(index);
	if (itr != options.end())
	{
		return itr->second;
	}
	return std::vector<std::string>();
}

bool Window::CommandArgs::hasOption(const std::string& index) const
{
	return (options.find(index) != options.end());
}

void Window::CommandArgs::parse(const std::string& commandArgs)
{
	args.clear();
	options.clear();
	if (commandArgs != "")
	{
		bool open = false;
		bool readParam = true;
		bool readOption = false;
		bool readArgs = false;
		std::size_t readPos = 0;
		std::string actualOption = "";

		for (std::size_t i = 0; i < commandArgs.size(); i++)
		{
			if (commandArgs[i] == ' ' && readParam)
			{
				if (readPos == 0)
				{
					args.push_back(commandArgs.substr(readPos, i - readPos));
				}
				else
				{
					args.push_back(commandArgs.substr(readPos + 1, i - readPos - 1));
				}
				readPos = i;
			}
			else if (commandArgs[i] == '-' && !open)
			{
				readArgs = false;
				readParam = false;
				readOption = true;
				readPos = i;
			}
			else if (commandArgs[i] == ' ' && readOption)
			{
				actualOption = commandArgs.substr(readPos + 1, i - readPos - 1);
				options[actualOption].clear();
				readOption = false;
				readArgs = true;
				readPos = i;
			}
			else if (commandArgs[i] == ' ' && readArgs && !open)
			{
				std::string arg = commandArgs.substr(readPos + 1, i - readPos - 1);
				if (arg != "")
				{
					if (arg[0] == '\"' && arg[arg.size() - 1] == '\"')
					{
						arg = arg.substr(1, arg.size() - 2);
					}
					options[actualOption].push_back(arg);
				}
				readPos = i;
			}
			else if (commandArgs[i] == '\"' && readArgs)
			{
				open = !open;
			}
		}

		if (readParam)
		{
			args.push_back(commandArgs.substr(readPos + ((readPos == 0) ? 0 : 1)));
		}
		else if (readOption)
		{
			options[commandArgs.substr(readPos + 1)].clear();
		}
		else if (readArgs)
		{
			std::string arg = commandArgs.substr(readPos + 1);
			if (arg != "")
			{
				if (arg[0] == '\"' && arg[arg.size() - 1] == '\"')
				{
					arg = arg.substr(1, arg.size() - 2);
				}
				options[actualOption].push_back(arg);
			}
		}
	}
}

void Window::setConsoleFont(const std::string& font)
{
	if (!mConsoleFontCreated)
	{
		mConsoleFont = new sf::Font();
		mConsoleFontCreated = true;
	}
	if (mConsoleFont->loadFromFile(font))
	{
		mConsoleFontData = font;
		mConsoleText.setFont(*mConsoleFont);
	}
	else
	{
		mConsoleFontData = "";
		if (mWarningCallback)
		{
			mWarningCallback("Can't find console font : " + font);
		}
	}
}

void Window::setConsoleFont(sf::Font* font, const std::string& fontData)
{
	if (mConsoleFontCreated)
	{
		delete mConsoleFont;
		mConsoleFont = nullptr;
		mConsoleFontCreated = false;
	}
	mConsoleFont = font;
	mConsoleFontData = fontData;
	mConsoleText.setFont(*mConsoleFont);
	mConsoleDisplayText.setFont(*mConsoleFont);
	mConsoleDisplayText.setString("AZERTYUIOPQSDFGHJKLMWXCVBN");
	mConsoleDisplayLineHeight = mConsoleDisplayText.getGlobalBounds().height;
	mConsoleDisplayText.setString("");
}

sf::Font* Window::getConsoleFont() const
{
	return mConsoleFont;
}

const std::string& Window::getConsoleFontData() const
{
	return mConsoleFontData;
}

void Window::setConsoleVisible(bool visible)
{
	mConsoleVisible = visible;
}

bool Window::isConsoleVisible() const
{
	return mConsoleVisible;
}

std::string Window::getConsoleContent() const
{
	return mConsoleContentBefore + mConsoleContentAfter;
}

void Window::setConsoleContent(const std::string& content)
{
	mConsoleContentBefore = content;
	mConsoleContentAfter = "";
}

void Window::setConsoleCommand(const std::string& name, std::function<void(const CommandArgs&)> command, const std::string& help, const std::string& man)
{
	Command cmd;
	cmd.name = name;
	cmd.command = command;
	cmd.help = help;
	cmd.man = man;
	mConsoleCommands.push_back(cmd);
}

bool Window::hasConsoleCommand(const std::string& name)
{
	for (std::size_t i = 0; i < mConsoleCommands.size(); i++)
	{
		if (mConsoleCommands[i].name == name)
		{
			return true;
		}
	}
	return false;
}

void Window::useConsoleCommand(const std::string& name, const std::string& args)
{
	for (std::size_t i = 0; i < mConsoleCommands.size(); i++)
	{
		if (mConsoleCommands[i].name == name && mConsoleCommands[i].command)
		{
			CommandArgs commandArgs;
			commandArgs.parse(args);
			mConsoleCommands[i].command(commandArgs);
		}
	}
}

void Window::removeConsoleCommand(const std::string& name)
{
	for (std::size_t i = 0; i < mConsoleCommands.size(); i++)
	{
		if (mConsoleCommands[i].name == name)
		{
			mConsoleCommands.erase(mConsoleCommands.begin() + i);
			i--;
		}
	}
}

void Window::addConsoleLine(const std::string& line)
{
	mConsoleDisplayLines.push_back(line);
	if (mConsoleDisplayLines.size() > 100)
	{
		mConsoleDisplayLines.erase(mConsoleDisplayLines.begin());
	}
}

void Window::setDebugInfoFont(const std::string& font)
{
	if (!mDebugInfoFontCreated)
	{
		mDebugInfoFont = new sf::Font();
		mDebugInfoFontCreated = true;
	}
	if (mDebugInfoFont->loadFromFile(font))
	{
		mDebugInfoFontData = font;
		mDebugInfoText.setFont(*mDebugInfoFont);
	}
	else
	{
		mDebugInfoFontData = "";
	}
}

void Window::setDebugInfoFont(sf::Font* font, const std::string& fontData)
{
	if (mDebugInfoFontCreated)
	{
		delete mDebugInfoFont;
		mDebugInfoFont = nullptr;
		mDebugInfoFontCreated = false;
	}
	mDebugInfoFont = font;
	mDebugInfoFontData = fontData;
	mDebugInfoText.setFont(*mDebugInfoFont);
}

sf::Font* Window::getDebugInfoFont() const
{
	return mDebugInfoFont;
}

const std::string& Window::getDebugInfoFontData() const
{
	return mDebugInfoFontData;
}

void Window::setDebugInfoVisible(bool visible)
{
	mDebugInfoVisible = visible;
}

bool Window::isDebugInfoVisible() const
{
	return mDebugInfoVisible;
}

void Window::setDebugInfoCharacterSize(unsigned int size)
{
	mDebugInfoText.setCharacterSize(size);
}

unsigned int Window::getDebugInfoCharacterSize() const
{
	return mDebugInfoText.getCharacterSize();
}

void Window::setDebugInfo(const std::string& id, const std::string& value)
{
	mDebugInfo[id] = value;
}

void Window::removeDebugInfo(const std::string& id)
{
	auto itr = mDebugInfo.find(id);
	if (itr != mDebugInfo.end())
	{
		mDebugInfo.erase(itr);
	}
}

unsigned int Window::getFPS() const
{
	return mLastFpsCounter;
}

void Window::setResizeCallback(std::function<void(const sf::Vector2u&)> resizeCallback)
{
	mResizeCallback = resizeCallback;
}

void Window::setErrorCallback(std::function<void(const std::string&)> errorCallback)
{
	mErrorCallback = errorCallback;
}

void Window::setWarningCallback(std::function<void(const std::string&)> warningCallback)
{
	mWarningCallback = warningCallback;
}

void Window::setInfoCallback(std::function<void(const std::string&)> infoCallback)
{
	mInfoCallback = infoCallback;
}

void Window::setLostFocusCallback(std::function<void()> lostFocusCallback)
{
	mLostFocusCallback = lostFocusCallback;
}

void Window::setGainedFocusCallback(std::function<void()> gainedFocusCallback)
{
	mGainedFocusCallback = gainedFocusCallback;
}

void Window::setCloseCallback(std::function<bool()> closeCallback)
{
	mCloseCallback = closeCallback;
}

void Window::setAction(Action action, sf::Keyboard::Key key)
{
	if (action >= 0 && action < Action::Count)
	{
		mActions[action] = key;
	}
}

sf::Keyboard::Key Window::getAction(Action action) const
{
	if (action >= 0 && action < Action::Count)
	{
		return mActions.at(action);
	}
	return sf::Keyboard::Unknown;
}

sf::RenderWindow& Window::getHandle()
{
	return mWindow;
}

void Window::onResize()
{
	// Update Console
	sf::Vector2u size = getSize();
	mConsoleText.setPosition(5.f, static_cast<float>(size.y) - 5.f);
	mConsoleBackground.setPosition(0.f, static_cast<float>(size.y));
	mConsoleBackground.setSize(sf::Vector2f(static_cast<float>(size.x), mConsoleBackground.getSize().y));
	mConsoleDisplayBackground.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y) * 0.5f));
	mConsoleDisplayBackground.setPosition(0.f, static_cast<float>(size.y) * 0.5f);

	// Callback
	if (mResizeCallback)
	{
		mResizeCallback(size);
	}
}

bool Window::pollEvent(sf::Event& event)
{
	bool ok = true;
	bool ret = false;
	do
	{
		ok = true;
		ret = mWindow.pollEvent(event);
		if (ret)
		{
			if (handleInternalEvent(event))
			{
				ok = false;
			}
		}
	} while (!ok);
	return ret;
}

bool Window::waitEvent(sf::Event& event)
{
	bool ret = mWindow.waitEvent(event);
	if (ret && handleInternalEvent(event))
	{
		return false;
	}
	return ret;
}

bool Window::handleInternalEvent(const sf::Event& event)
{
	// Close
	if (event.type == sf::Event::Closed)
	{
		close();
	}

	// Lost Focus
	if (event.type == sf::Event::LostFocus && mLostFocusCallback)
	{
		mLostFocusCallback();
	}

	// Gained Focus
	if (event.type == sf::Event::GainedFocus && mGainedFocusCallback)
	{
		mGainedFocusCallback();
	}

	bool action = false;
	if (event.type == sf::Event::KeyPressed && event.key.code == mActions[Action::Console] && mActions[Action::Console] != sf::Keyboard::Unknown)
	{
		mConsoleVisible = !mConsoleVisible;
		action = true;
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == mActions[Action::DebugInfo] && mActions[Action::DebugInfo] != sf::Keyboard::Unknown)
	{
		mDebugInfoVisible = !mDebugInfoVisible;
		action = true;
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == mActions[Action::Quit] && mActions[Action::Quit] != sf::Keyboard::Unknown)
	{
		close();
		action = true;
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == mActions[Action::Fullscreen] && mActions[Action::Fullscreen] != sf::Keyboard::Unknown)
	{
		toggleFullscreen();
		action = true;
	}

	if (action)
	{
		return true;
	}

	if (isConsoleVisible())
	{
		// MouseWheelScrolled on console
		if (event.type == sf::Event::MouseWheelScrolled
		    && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel
		    && mConsoleDisplayBackground.getGlobalBounds().contains(getMousePosition()))
		{
			int d = static_cast<int>(event.mouseWheelScroll.delta);
			if (event.mouseWheelScroll.delta > 0.f && mConsoleDisplayBegin + d >= 0)
			{
				mConsoleDisplayBegin -= d;
			}
			else if (event.mouseWheelScroll.delta < 0.f && mConsoleDisplayBegin - d < mConsoleDisplayLines.size())
			{
				mConsoleDisplayBegin -= d;
			}
		}

		// Key : Delete, Left Arrow, Right Arrow
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete)
		{
			if (mConsoleContentAfter.size() >= 1)
			{
				mConsoleContentAfter.erase(mConsoleContentAfter.begin());
				return true;
			}
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
		{
			if (mConsoleContentBefore.size() >= 1)
			{
				mConsoleContentAfter.insert(mConsoleContentAfter.begin(), mConsoleContentBefore.back());
				mConsoleContentBefore.pop_back();
				return true;
			}
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
		{
			if (mConsoleContentAfter.size() >= 1)
			{
				mConsoleContentBefore += mConsoleContentAfter[0];
				mConsoleContentAfter.erase(mConsoleContentAfter.begin());
				return true;
			}
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
		{
			std::string command = getConsoleContent();
			std::string args = "";
			mConsoleContentAfter.clear();
			mConsoleContentBefore.clear();
			addConsoleLine(command);
			std::size_t found = command.find_first_of(" ");
			if (found != std::string::npos)
			{
				args = command.substr(found + 1);
				command = command.substr(0, found);
			}
			useConsoleCommand(command, args);
			return true;
		}
		else if (event.type == sf::Event::TextEntered && event.text.unicode < 128 && event.text.unicode != 13)
		{
			// Text : Return, Backspace & others
			char c = static_cast<char>(event.text.unicode);
			switch (c)
			{
				case '\b':
				{
					if (mConsoleContentBefore.size() >= 1)
					{
						mConsoleContentBefore.pop_back();
						return true;
					}
				} break;

				default:
				{
					mConsoleContentBefore += c;
				} break;
			}
		}

		
	}
	return false;
}

Window::RWindow::RWindow(ke::Window& handle) : mHandle(handle)
{
}

void Window::RWindow::onResize()
{
	mHandle.onResize();
}

void Window::RWindow::init()
{
	sf::RenderTarget::initialize();
}

} // namespace ke