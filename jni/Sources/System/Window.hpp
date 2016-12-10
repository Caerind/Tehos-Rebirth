#ifndef KE_WINDOW_HPP
#define KE_WINDOW_HPP

#include <array>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Touch.hpp>

namespace ke
{

class Window
{
	public:
		Window();
		~Window();

		void create();
		void create(sf::VideoMode mode, const std::string& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
		void toFullscreen();
		void toNonFullscreen();
		void toggleFullscreen();
		void close();

		bool isFullscreen() const;
		const sf::VideoMode& getVideoMode() const;
		sf::Uint32 getStyle() const;
		const std::string& getTitle() const;
		bool isVerticalSyncEnabled() const;
		bool isVisible() const;
		bool isKeyRepeatEnabled() const;
		float getJoystickThreshold() const;
		bool isMouseCursorGrabbed() const;
		const sf::Image& getIcon() const;
		const std::string& getIconPath() const;
		sf::Vector2i getPosition() const;
		const sf::ContextSettings& getSettings() const;
		sf::Vector2u getSize() const;
		sf::WindowHandle getSystemHandle() const;
		bool hasFocus() const;

		void setVerticalSyncEnabled(bool enabled);
		void setVisible(bool visible);
		void setKeyRepeatEnabled(bool enabled);
		void setJoystickThreshold(float threshold);
		void setMouseCursorGrabbed(bool grabbed);
		void setIcon(const std::string& icon);
		void setIcon(const sf::Image& image);
		void setPosition(const sf::Vector2i& position);
		void setSize(const sf::Vector2u& size);
		void setActive(bool active = true);
		void requestFocus();

		bool isOpen() const;
		bool handleEvent(sf::Event& event);
		void clear(const sf::Color& color = sf::Color(0, 0, 0, 255));
		void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
		void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
		void display();

		void setView(const sf::View& view);
		void applyDefaultView();
		const sf::View& getView() const;
		const sf::View& getDefaultView() const;
		sf::IntRect getViewport(const sf::View& view) const; 
		void mapPixelToCoords(const sf::Vector2i& point, const sf::View& view = sf::View(sf::FloatRect()));
		void mapCoordsToPixel(const sf::Vector2f& point, const sf::View& view = sf::View(sf::FloatRect()));
		
		void setMousePosition(const sf::Vector2f& position);
		void setMousePositionView(const sf::Vector2f& position, const sf::View& view);
		sf::Vector2f getMousePosition() const;
		sf::Vector2f getMousePositionView(const sf::View& view);
		sf::Vector2f getTouchPosition(unsigned int touchIndex = 0) const;
		sf::Vector2f getTouchPositionView(const sf::View& view, unsigned int touchIndex = 0);
		void setPointerPosition(const sf::Vector2f& position);
		void setPointerPositionView(const sf::Vector2f& position, const sf::View& view);
		sf::Vector2f getPointerPosition(unsigned int touchIndex = 0) const;
		sf::Vector2f getPointerPositionView(const sf::View& view, unsigned int touchIndex = 0);

		enum MouseCursor { Default = 0, None, Custom };
		MouseCursor getMouseCursor() const;
		void setMouseCursor(MouseCursor mouseCursor);
		void hideMouseCursor();
		void useDefaultMouseCursor();
		void useCustomMouseCursor(const std::string& texture, const sf::IntRect& textureRect = sf::IntRect(), const sf::Vector2f& origin = sf::Vector2f());
		void useCustomMouseCursor(sf::Texture* texture, const std::string& textureData = "", const sf::IntRect& textureRect = sf::IntRect(), const sf::Vector2f& origin = sf::Vector2f());
		sf::Texture* getMouseCursorTexture() const; 
		const std::string& getMouseCursorTextureData() const;
		const sf::IntRect& getMouseCursorTextureRect() const;
		const sf::Vector2f& getMouseCursorOrigin() const;
		const sf::Vector2f& getMouseCursorScale() const;
		float getMouseCursorRotation() const;
		const sf::Color& getMouseCursorColor() const;
		void setMouseCursorTexture(const std::string& texture);
		void setMouseCursorTexture(sf::Texture* texture, const std::string& textureData = "");
		void setMouseCursorTextureRect(const sf::IntRect& textureRect);
		void setMouseCursorOrigin(const sf::Vector2f& origin);
		void setMouseCursorScale(const sf::Vector2f& scale);
		void setMouseCursorRotation(float rotation);
		void setMouseCursorColor(const sf::Color& color);

		void screenshot();
		const std::string& getScreenshotPath() const;
		void setScreenshotPath(const std::string& screenshotPath);

		enum BackgroundUsage { Color = 0, Scaled, Repeated };
		BackgroundUsage getBackgroundUsage() const;
		void setBackgroundUsage(BackgroundUsage usage);
		void useBackgroundColor(const sf::Color& color);
		void useBackgroundScaled(const std::string& texture, const sf::IntRect& rect = sf::IntRect());
		void useBackgroundScaled(sf::Texture* texture, const std::string& textureData = "", const sf::IntRect& rect = sf::IntRect());
		void useBackgroundRepeated(const std::string& texture, const sf::IntRect& rect = sf::IntRect());
		void useBackgroundRepeated(sf::Texture* texture, const std::string& textureData = "", const sf::IntRect& rect = sf::IntRect());
		const sf::Color& getBackgroundColor() const;
		sf::Texture* getBackgroundTexture() const;
		const std::string& getBackgroundTextureData() const;
		const sf::IntRect& getBackgroundTextureRect() const;

		struct CommandArgs
		{
			CommandArgs();
			CommandArgs(const std::string& args);

			std::string operator[](std::size_t index) const;
			std::size_t argsCount() const;
			std::vector<std::string> args;

			std::vector<std::string> operator[](const std::string& index) const;
			bool hasOption(const std::string& index) const;
			std::map<std::string, std::vector<std::string>> options;

			void parse(const std::string& args);
		};

		void setConsoleFont(const std::string& font);
		void setConsoleFont(sf::Font* font, const std::string& fontData = "");
		sf::Font* getConsoleFont() const;
		const std::string& getConsoleFontData() const;
		void setConsoleVisible(bool visible);
		bool isConsoleVisible() const;
		std::string getConsoleContent() const;
		void setConsoleContent(const std::string& content);
		void setConsoleCommand(const std::string& name, std::function<void(const CommandArgs&)> command, const std::string& help = "", const std::string& man = "");
		bool hasConsoleCommand(const std::string& name);
		void useConsoleCommand(const std::string& name, const std::string& args = "");
		void removeConsoleCommand(const std::string& name);
		void addConsoleLine(const std::string& line);

		void setDebugInfoFont(const std::string& font);
		void setDebugInfoFont(sf::Font* font, const std::string& fontData = "");
		sf::Font* getDebugInfoFont() const;
		const std::string& getDebugInfoFontData() const;
		void setDebugInfoVisible(bool visible);
		bool isDebugInfoVisible() const;
		void setDebugInfoCharacterSize(unsigned int size);
		unsigned int getDebugInfoCharacterSize() const;
		void setDebugInfo(const std::string& id, const std::string& value);
		void removeDebugInfo(const std::string& id);

		unsigned int getFPS() const;

		void setResizeCallback(std::function<void(const sf::Vector2u&)> resizeCallback);
		void setErrorCallback(std::function<void(const std::string&)> errorCallback);
		void setWarningCallback(std::function<void(const std::string&)> warningCallback);
		void setInfoCallback(std::function<void(const std::string&)> infoCallback);
		void setLostFocusCallback(std::function<void()> lostFocusCallback);
		void setGainedFocusCallback(std::function<void()> gainedFocusCallback);
		void setCloseCallback(std::function<bool()> closeCallback);

		enum Action { Console = 0, Screenshot, DebugInfo, Quit, Fullscreen, Count};
		void setAction(Action action, sf::Keyboard::Key key = sf::Keyboard::Unknown);
		sf::Keyboard::Key getAction(Action action) const;

		sf::RenderWindow& getHandle();

	private:
		void onResize();

		bool pollEvent(sf::Event& event);
		bool waitEvent(sf::Event& event);
		bool handleInternalEvent(const sf::Event& event);

		class RWindow : public sf::RenderWindow
		{
			public:
				RWindow(ke::Window& handle);

				virtual void onResize();
				void init();

				friend void ke::Window::onResize();

			private:
				ke::Window& mHandle;
		};

		struct Command
		{
			std::string name;
			std::function<void(const CommandArgs&)> command;
			std::string help;
			std::string man;
		};

	private:
		RWindow mWindow;

		std::string mTitle;
		bool mVisible;
		bool mVerticalSyncEnabled;
		bool mKeyRepeatEnabled;
		float mJoystickThreshold;
		bool mMouseCursorGrabbed;
		sf::ContextSettings mSettings;
		bool mFullscreen;
		sf::VideoMode mFullscreenVideoMode;
		sf::VideoMode mNonFullscreenVideoMode;
		sf::Uint32 mNonFullscreenStyle;

		sf::Image mIcon;
		std::string mIconPath;

		MouseCursor mMouseCursor;
		std::string mMouseCursorTextureData;
		sf::Texture* mMouseCursorTexture;
		bool mMouseCursorTextureCreated;
		sf::Sprite mMouseCursorSprite;

		std::string mScreenshotPath;

		BackgroundUsage mBackgroundUsage;
		sf::Color mBackgroundColor;
		std::string mBackgroundTextureData;
		sf::Texture* mBackgroundTexture;
		bool mBackgroundTextureCreated;
		sf::Sprite mBackgroundSprite;

		std::string mConsoleFontData;
		sf::Font* mConsoleFont;
		bool mConsoleFontCreated;
		sf::Text mConsoleText;
		std::string mConsoleContentBefore;
		std::string mConsoleContentAfter;
		sf::Clock mConsoleBlinkTime;
		sf::RectangleShape mConsoleBackground;
		bool mConsoleVisible;
		std::vector<Command> mConsoleCommands;
		std::vector<std::string> mConsoleDisplayLines;
		sf::RectangleShape mConsoleDisplayBackground;
		sf::Text mConsoleDisplayText;
		std::size_t mConsoleDisplayBegin;
		float mConsoleDisplayLineHeight;

		bool mDebugInfoVisible;
		std::string mDebugInfoFontData;
		sf::Font* mDebugInfoFont;
		bool mDebugInfoFontCreated;
		sf::Text mDebugInfoText;
		std::unordered_map<std::string, std::string> mDebugInfo;

		sf::Clock mFpsClock;
		unsigned int mFpsCounter;
		unsigned int mLastFpsCounter;

		std::function<void(const sf::Vector2u&)> mResizeCallback;
		std::function<void(const std::string&)> mErrorCallback;
		std::function<void(const std::string&)> mWarningCallback;
		std::function<void(const std::string&)> mInfoCallback;
		std::function<void()> mLostFocusCallback;
		std::function<void()> mGainedFocusCallback;
		std::function<bool()> mCloseCallback;

		std::array<sf::Keyboard::Key, Action::Count> mActions;
};

} // namespace ke

#endif // KE_WINDOW_HPP