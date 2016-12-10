#ifndef KE_INPUTSYSTEM_HPP
#define KE_INPUTSYSTEM_HPP

#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Window/Event.hpp>

namespace ke
{

enum InputType { Pressed, Released, Hold };

struct KeyboardPair
{
	KeyboardPair() {}
	KeyboardPair(sf::Keyboard::Key k, InputType t) : key(k), type(t) {}

	sf::Keyboard::Key key;
	InputType type;

	std::string toString();
	void fromString(std::string const& str);
};

struct MousePair
{
	MousePair() {}
	MousePair(sf::Mouse::Button b, InputType t) : button(b), type(t) {}

	sf::Mouse::Button button;
	InputType type;

	std::string toString();
	void fromString(std::string const& str);
};

struct TouchPair
{
	TouchPair() {}
	TouchPair(unsigned int i, InputType t) : index(i), type(t) {}

	unsigned int index;
	InputType type;

	std::string toString();
	void fromString(std::string const& str);
};

class Input
{
	public:
		Input();
		~Input();

		void bindAction(std::string const& key, std::function<bool(std::vector<std::string> const& data)> func);
		void unbindAction(std::string const& key);
		bool hasAction(std::string const& key) const;

		bool handleAction(std::string const& key, std::vector<std::string> const& data);

		void setPriority(float priority);
		float getPriority() const;

	private:
		std::map<std::string, std::function<bool(std::vector<std::string> const& data)>> mActions;
		float mPriority;
};

class InputSystem
{
    public:
        InputSystem();
        ~InputSystem();

        void setEventMapping(std::string const& index, sf::Event::EventType eventType);
        sf::Event::EventType getEventMapping(std::string const& index);

        void setKeyboardMapping(std::string const& index, sf::Keyboard::Key key, InputType type);
        KeyboardPair getKeyboardMapping(std::string const& index);

        void setMouseMapping(std::string const& index, sf::Mouse::Button button, InputType type);
        MousePair getMouseMapping(std::string const& index);

		void setTouchMapping(std::string const& index, unsigned int touchIndex, InputType type);
		TouchPair getTouchMapping(std::string const& index);

        void handleEvent(sf::Event const& event);
        void update(sf::Time dt);

		void emitData(std::string const& key, std::vector<std::string> const& data = std::vector<std::string>());

        void registerInput(Input* input);
        void unregisterInput(Input* input);

        void bindAction(std::string const& key, std::function<bool(std::vector<std::string> const& data)> func);
        void unbindAction(std::string const& key);
        bool hasAction(std::string const& key) const;

		bool loadFromFile(std::string const& filename);
		bool saveToFile(std::string const& filename);

	private:
		std::vector<std::string> eventToData(sf::Event const& event);

	public:
		static std::string inputTypeToString(InputType type);
		static InputType stringToInputType(std::string const& str);

		static std::string eventTypeToString(sf::Event::EventType type);
		static sf::Event::EventType stringToEventType(std::string const& type);

		static std::string keyToString(sf::Keyboard::Key const& key);
		static sf::Keyboard::Key stringToKey(std::string const& str);

		static std::string buttonToString(sf::Mouse::Button const& button);
		static sf::Mouse::Button stringToButton(std::string const& button);

		static std::string unicodeToString(sf::Uint32 unicode);

    private:
		Input mDefaultInput;
        std::vector<Input*> mInputs;
        std::vector<sf::Event> mEvents;
		std::vector<std::pair<std::string, std::vector<std::string>>> mData;
        std::unordered_map<std::string, sf::Event::EventType> mEventMapping;
        std::unordered_map<std::string, KeyboardPair> mKeyboardMapping;
        std::unordered_map<std::string, MousePair> mMouseMapping;
		std::unordered_map<std::string, TouchPair> mTouchMapping;
};

} // namespace ke

#endif // KE_INPUTSYSTEM_HPP
