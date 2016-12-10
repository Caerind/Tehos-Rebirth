#include "InputSystem.hpp"
#include "Application.hpp"

namespace ke
{

std::string KeyboardPair::toString()
{
	return "type=Keyboard&key=" + InputSystem::keyToString(key) + "&input=" + InputSystem::inputTypeToString(type);
}

void KeyboardPair::fromString(std::string const& str)
{
	std::stringstream ss(str);
	std::string temp;
	std::size_t i = 0;
	while (std::getline(ss, temp, '&'))
	{
		std::size_t p = temp.find('=');
		if (p != std::string::npos)
		{
			if (i == 1)
			{
				key = InputSystem::stringToKey(temp.substr(p + 1));
			}
			if (i == 2)
			{
				type = InputSystem::stringToInputType(temp.substr(p + 1));
			}
		}
		i++;
	}
}

std::string MousePair::toString()
{
	return "type=Mouse&button=" + InputSystem::buttonToString(button) + "&input=" + InputSystem::inputTypeToString(type);
}

void MousePair::fromString(std::string const& str)
{
	std::stringstream ss(str);
	std::string temp;
	std::size_t i = 0;
	while (std::getline(ss, temp, '&'))
	{
		std::size_t p = temp.find('=');
		if (p != std::string::npos)
		{
			if (i == 1)
			{
				button = InputSystem::stringToButton(temp.substr(p + 1));
			}
			if (i == 2)
			{
				type = InputSystem::stringToInputType(temp.substr(p + 1));
			}
		}
		i++;
	}
}

std::string TouchPair::toString()
{
	return "type=Touch&index=" + ke::toString(index) + "&input=" + InputSystem::inputTypeToString(type);
}

void TouchPair::fromString(std::string const& str)
{
	std::stringstream ss(str);
	std::string temp;
	std::size_t i = 0;
	while (std::getline(ss, temp, '&'))
	{
		std::size_t p = temp.find('=');
		if (p != std::string::npos)
		{
			if (i == 1)
			{
				index = (temp.substr(p + 1) == "0") ? 0 : 1;
			}
			if (i == 2)
			{
				type = InputSystem::stringToInputType(temp.substr(p + 1));
			}
		}
		i++;
	}
}

Input::Input()
	: mActions()
	, mPriority(0.f)
{
}

Input::~Input()
{
}

void Input::bindAction(std::string const& key, std::function<bool(std::vector<std::string> const& data)> func)
{
	mActions[key] = func;
}

void Input::unbindAction(std::string const& key)
{
	auto itr = mActions.find(key);
	if (itr != mActions.end())
	{
		mActions.erase(itr);
	}
}

bool Input::hasAction(std::string const& key) const
{
	return (mActions.find(key) != mActions.end());
}

bool Input::handleAction(std::string const& key, std::vector<std::string> const& data)
{
	auto itr = mActions.find(key);
	if (itr != mActions.end())
	{
		if (itr->second)
		{
			return itr->second(data);
		}
	}
	return false;
}

void Input::setPriority(float priority)
{
	mPriority = priority;
}

float Input::getPriority() const
{
	return mPriority;
}

InputSystem::InputSystem()
{
	mInputs.push_back(&mDefaultInput);
}

InputSystem::~InputSystem()
{
}

void InputSystem::setEventMapping(std::string const& index, sf::Event::EventType eventType)
{
    mEventMapping[index] = eventType;
}

sf::Event::EventType InputSystem::getEventMapping(std::string const& index)
{
    return mEventMapping[index];
}

void InputSystem::setKeyboardMapping(std::string const& index, sf::Keyboard::Key key, InputType type)
{
    mKeyboardMapping[index] = KeyboardPair(key, type);
}

KeyboardPair InputSystem::getKeyboardMapping(std::string const& index)
{
    return mKeyboardMapping[index];
}

void InputSystem::setMouseMapping(std::string const& index, sf::Mouse::Button button, InputType type)
{
    mMouseMapping[index] = MousePair(button, type);
}

MousePair InputSystem::getMouseMapping(std::string const& index)
{
    return mMouseMapping[index];
}

void InputSystem::setTouchMapping(std::string const& index, unsigned int touchIndex, InputType type)
{
	mTouchMapping[index] = TouchPair(touchIndex, type);
}

TouchPair InputSystem::getTouchMapping(std::string const & index)
{
	return mTouchMapping[index];
}

void InputSystem::handleEvent(sf::Event const& event)
{
    mEvents.push_back(event);
}

void InputSystem::update(sf::Time dt)
{
	// Sort inputs
	std::sort(mInputs.begin(), mInputs.end(), [](Input* a, Input* b)
	{
		return a->getPriority() > b->getPriority();
	});

	// Process events
	std::size_t size = mEvents.size();
	for (std::size_t i = 0; i < size; i++)
	{
		std::vector<std::string> data = eventToData(mEvents[i]);
		// Normal event
		for (auto itr = mEventMapping.begin(); itr != mEventMapping.end(); itr++)
		{
			if (itr->second == mEvents[i].type)
			{
				emitData(itr->first, data);
			}
		}
		// Key pressed/released event
		if (mEvents[i].type == sf::Event::KeyPressed || mEvents[i].type == sf::Event::KeyReleased)
		{
			for (auto itr = mKeyboardMapping.begin(); itr != mKeyboardMapping.end(); itr++)
			{
				if (itr->second.type != InputType::Hold)
				{
					bool type = false;
					if ((itr->second.type == InputType::Pressed && mEvents[i].type == sf::Event::KeyPressed)
						|| (itr->second.type == InputType::Released && mEvents[i].type == sf::Event::KeyReleased))
					{
						type = true;
					}
					if (mEvents[i].key.code == itr->second.key && type)
					{
						emitData(itr->first, data);
					}
				}
			}
		}
		// Mouse pressed/released event
		if (mEvents[i].type == sf::Event::MouseButtonPressed || mEvents[i].type == sf::Event::MouseButtonReleased)
		{
			for (auto itr = mMouseMapping.begin(); itr != mMouseMapping.end(); itr++)
			{
				if (itr->second.type != InputType::Hold)
				{
					bool type = false;
					if ((itr->second.type == InputType::Pressed && mEvents[i].type == sf::Event::MouseButtonPressed)
						|| (itr->second.type == InputType::Released && mEvents[i].type == sf::Event::MouseButtonReleased))
					{
						type = true;
					}
					if (mEvents[i].mouseButton.button == itr->second.button && type)
					{
						emitData(itr->first, data);
					}
				}
			}
		}

		// Touch began/ended event
		if (mEvents[i].type == sf::Event::TouchBegan || mEvents[i].type == sf::Event::TouchEnded)
		{
			for (auto itr = mTouchMapping.begin(); itr != mTouchMapping.end(); itr++)
			{
				if (itr->second.type != InputType::Hold)
				{
					bool type = false;
					if ((itr->second.type == InputType::Pressed && mEvents[i].type == sf::Event::TouchBegan)
						|| (itr->second.type == InputType::Released && mEvents[i].type == sf::Event::TouchEnded))
					{
						type = true;
					}
					if (mEvents[i].touch.finger == itr->second.index && type)
					{
						emitData(itr->first, data);
					}
				}
			}
		}
	}
	mEvents.clear();

	std::vector<std::string> realtimeData;
	realtimeData.push_back(ke::toString(dt.asSeconds()));

	// Keyboard realtime
	for (auto itr = mKeyboardMapping.begin(); itr != mKeyboardMapping.end(); itr++)
	{
		if (sf::Keyboard::isKeyPressed(itr->second.key) && itr->second.type == InputType::Hold)
		{
			emitData(itr->first, realtimeData);
		}
	}

	// Mouse realtime
	for (auto itr = mMouseMapping.begin(); itr != mMouseMapping.end(); itr++)
	{
		auto mouseData = realtimeData;
		auto mousePos = Application::getWindow().getMousePosition();
		mouseData.push_back(ke::toString(mousePos.x));
		mouseData.push_back(ke::toString(mousePos.y));
		if (sf::Mouse::isButtonPressed(itr->second.button) && itr->second.type == InputType::Hold)
		{
			emitData(itr->first, mouseData);
		}
	}

	// Touch realtime
	for (auto itr = mTouchMapping.begin(); itr != mTouchMapping.end(); itr++)
	{
		auto touchData = realtimeData;
		auto touchPos = Application::getWindow().getTouchPosition();
		touchData.push_back(ke::toString(touchPos.x));
		touchData.push_back(ke::toString(touchPos.y));
		if (sf::Touch::isDown(itr->second.index) && itr->second.type == InputType::Hold)
		{
			emitData(itr->first, touchData);
		}
	}

	// Process data
	size = mData.size();
	for (std::size_t i = 0; i < size; i++)
	{
		std::string key = mData[i].first;
		std::vector<std::string> data = mData[i].second;
		std::size_t size2 = mInputs.size();
		bool handled = false;
		for (std::size_t j = 0; j < size2; j++)
		{
			if (!handled && mInputs[j]->hasAction(key))
			{
				handled = mInputs[j]->handleAction(key, data);
			}
		}
	}
	mData.clear();
}

void InputSystem::emitData(std::string const& key, std::vector<std::string> const& data)
{
	mData.push_back(std::pair<std::string,std::vector<std::string>>(key, data));
}

void InputSystem::registerInput(Input* input)
{
    mInputs.push_back(input);
}

void InputSystem::unregisterInput(Input* input)
{
    for (std::size_t i = 0; i < mInputs.size();)
    {
        if (mInputs[i] == input)
        {
            mInputs.erase(mInputs.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void InputSystem::bindAction(std::string const& key, std::function<bool(std::vector<std::string> const& data)> func)
{
    mDefaultInput.bindAction(key,func);
}

void InputSystem::unbindAction(std::string const& key)
{
	mDefaultInput.unbindAction(key);
}

bool InputSystem::hasAction(std::string const& key) const
{
    return mDefaultInput.hasAction(key);
}

bool InputSystem::loadFromFile(std::string const & filename)
{
	std::ifstream file(filename);
	if (file)
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::string key;
			std::string content;
			std::size_t found = line.find_first_of(":");
			if (found != std::string::npos)
			{
				key = line.substr(0, found);
				content = line.substr(found + 1);
				if (content.find("Mouse") != std::string::npos)
				{
					mMouseMapping[key].fromString(content);
				}
				else if (content.find("Keyboard") != std::string::npos)
				{
					mKeyboardMapping[key].fromString(content);
				}
				else
				{
					mEventMapping[key] = stringToEventType(content);
				}
			}
		}
	}
	return false;
}

bool InputSystem::saveToFile(std::string const & filename)
{
	std::ofstream file(filename);
	if (file)
	{
		for (auto itr = mEventMapping.begin(); itr != mEventMapping.end(); itr++)
		{
			file << itr->first << ":" << eventTypeToString(itr->second) << std::endl;
		}
		for (auto itr = mKeyboardMapping.begin(); itr != mKeyboardMapping.end(); itr++)
		{
			file << itr->first << ":" << itr->second.toString() << std::endl;
		}
		for (auto itr = mMouseMapping.begin(); itr != mMouseMapping.end(); itr++)
		{
			file << itr->first << ":" << itr->second.toString() << std::endl;
		}
		return true;
	}
	return false;
}

std::vector<std::string> InputSystem::eventToData(sf::Event const& event)
{
	std::vector<std::string> data;
	data.push_back(eventTypeToString(event.type));

	if (event.type == sf::Event::Resized)
	{
		data.push_back(ke::toString(event.size.width));
		data.push_back(ke::toString(event.size.height));
	}
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
	{
		data.push_back(keyToString(event.key.code));
		data.push_back(ke::toString(event.key.alt));
		data.push_back(ke::toString(event.key.control));
		data.push_back(ke::toString(event.key.shift));
		data.push_back(ke::toString(event.key.system));
	}
	if (event.type == sf::Event::TextEntered)
	{
		data.push_back(unicodeToString(event.text.unicode));
	}
	if (event.type == sf::Event::MouseMoved)
	{
		data.push_back(ke::toString(event.mouseMove.x));
		data.push_back(ke::toString(event.mouseMove.y));
	}
	if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
	{
		data.push_back(ke::toString(event.mouseButton.button));
		data.push_back(ke::toString(event.mouseButton.x));
		data.push_back(ke::toString(event.mouseButton.y));
	}
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		data.push_back(ke::toString(event.mouseWheelScroll.wheel));
		data.push_back(ke::toString(event.mouseWheelScroll.delta));
		data.push_back(ke::toString(event.mouseWheelScroll.x));
		data.push_back(ke::toString(event.mouseWheelScroll.y));
	}
	if (event.type == sf::Event::MouseWheelMoved)
	{
		data.push_back(ke::toString(event.mouseWheel.delta));
		data.push_back(ke::toString(event.mouseWheel.x));
		data.push_back(ke::toString(event.mouseWheel.y));
	}
	if (event.type == sf::Event::JoystickMoved)
	{
		data.push_back(ke::toString(event.joystickMove.joystickId));
		data.push_back(ke::toString(event.joystickMove.axis));
		data.push_back(ke::toString(event.joystickMove.position));
	}
	if (event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased)
	{
		data.push_back(ke::toString(event.joystickButton.joystickId));
		data.push_back(ke::toString(event.joystickButton.button));
	}
	if (event.type == sf::Event::JoystickConnected || event.type == sf::Event::JoystickDisconnected)
	{
		data.push_back(ke::toString(event.joystickConnect.joystickId));
	}
	if (event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded || event.type == sf::Event::TouchMoved)
	{
		data.push_back(ke::toString(event.touch.finger));
		data.push_back(ke::toString(event.touch.x));
		data.push_back(ke::toString(event.touch.y));
	}
	if (event.type == sf::Event::SensorChanged)
	{
		data.push_back(ke::toString(event.sensor.type));
		data.push_back(ke::toString(event.sensor.x));
		data.push_back(ke::toString(event.sensor.y));
		data.push_back(ke::toString(event.sensor.z));
	}

	return data;
}

std::string InputSystem::inputTypeToString(InputType type)
{
	switch (type)
	{
		case InputType::Pressed: return "Pressed"; break;
		case InputType::Released: return "Released"; break;
		case InputType::Hold: return "Hold"; break;
		default: break;
	}
	return "Pressed";
}

InputType InputSystem::stringToInputType(std::string const& str)
{
	if (str == "Hold")
	{
		return InputType::Hold;
	}
	else if (str == "Released")
	{
		return InputType::Released;
	}
	else
	{
		return InputType::Pressed;
	}
}

std::string InputSystem::eventTypeToString(sf::Event::EventType type)
{
	switch (type)
	{
		case sf::Event::Closed: return "Closed"; break;
		case sf::Event::Resized: return "Resized"; break;
		case sf::Event::LostFocus: return "LostFocus"; break;
		case sf::Event::GainedFocus: return "GainedFocus"; break;
		case sf::Event::TextEntered: return "TextEntered"; break;
		case sf::Event::KeyPressed: return "KeyPressed"; break;
		case sf::Event::KeyReleased: return "KeyReleased"; break;
		case sf::Event::MouseWheelMoved: return "MouseWheelMoved"; break;
		case sf::Event::MouseWheelScrolled: return "MouseWheelScrolled"; break;
		case sf::Event::MouseButtonPressed: return "MouseButtonPressed"; break;
		case sf::Event::MouseButtonReleased: return "MouseButtonReleased"; break;
		case sf::Event::MouseMoved: return "MouseMoved"; break;
		case sf::Event::MouseEntered: return "MouseEntered"; break;
		case sf::Event::MouseLeft: return "MouseLeft"; break;
		case sf::Event::JoystickButtonPressed: return "JoystickButtonPressed"; break;
		case sf::Event::JoystickButtonReleased: return "JoystickButtonReleased"; break;
		case sf::Event::JoystickMoved: return "JoystickMoved"; break;
		case sf::Event::JoystickConnected: return "JoystickConnected"; break;
		case sf::Event::JoystickDisconnected: return "JoystickDisconnected"; break;
		case sf::Event::TouchBegan: return "TouchBegan"; break;
		case sf::Event::TouchMoved: return "TouchMoved"; break;
		case sf::Event::TouchEnded: return "TouchEnded"; break;
		case sf::Event::SensorChanged: return "SensorChanged"; break;
		default: return "Unknown"; break;
	}
}

sf::Event::EventType InputSystem::stringToEventType(std::string const& type)
{
	if (type == "Closed") { return sf::Event::Closed; }
	if (type == "Resized") { return sf::Event::Resized; }
	if (type == "LostFocus") { return sf::Event::LostFocus; }
	if (type == "GainedFocus") { return sf::Event::GainedFocus; }
	if (type == "TextEntered") { return sf::Event::TextEntered; }
	if (type == "KeyPressed") { return sf::Event::KeyPressed; }
	if (type == "KeyReleased") { return sf::Event::KeyReleased; }
	if (type == "MouseWheelMoved") { return sf::Event::MouseWheelMoved; }
	if (type == "MouseWheelScrolled") { return sf::Event::MouseWheelScrolled; }
	if (type == "MouseButtonPressed") { return sf::Event::MouseButtonPressed; }
	if (type == "MouseButtonReleased") { return sf::Event::MouseButtonReleased; }
	if (type == "MouseMoved") { return sf::Event::MouseMoved; }
	if (type == "MouseEntered") { return sf::Event::MouseEntered; }
	if (type == "MouseLeft") { return sf::Event::MouseLeft; }
	if (type == "JoystickButtonPressed") { return sf::Event::JoystickButtonPressed; }
	if (type == "JoystickButtonReleased") { return sf::Event::JoystickButtonReleased; }
	if (type == "JoystickMoved") { return sf::Event::JoystickMoved; }
	if (type == "JoystickConnected") { return sf::Event::JoystickConnected; }
	if (type == "JoystickDisconnected") { return sf::Event::JoystickDisconnected; }
	if (type == "TouchBegan") { return sf::Event::TouchBegan; }
	if (type == "TouchMoved") { return sf::Event::TouchMoved; }
	if (type == "TouchEnded") { return sf::Event::TouchEnded; }
	if (type == "SensorChanged") { return sf::Event::SensorChanged; }
	return sf::Event::Count;
}

std::string InputSystem::keyToString(sf::Keyboard::Key const& key)
{
	switch (key)
	{
		case sf::Keyboard::Unknown: return "Unknown"; break;
		case sf::Keyboard::A: return "A"; break;
		case sf::Keyboard::B: return "B"; break;
		case sf::Keyboard::C: return "C"; break;
		case sf::Keyboard::D: return "D"; break;
		case sf::Keyboard::E: return "E"; break;
		case sf::Keyboard::F: return "F"; break;
		case sf::Keyboard::G: return "G"; break;
		case sf::Keyboard::H: return "H"; break;
		case sf::Keyboard::I: return "I"; break;
		case sf::Keyboard::J: return "J"; break;
		case sf::Keyboard::K: return "K"; break;
		case sf::Keyboard::L: return "L"; break;
		case sf::Keyboard::M: return "M"; break;
		case sf::Keyboard::N: return "N"; break;
		case sf::Keyboard::O: return "O"; break;
		case sf::Keyboard::P: return "P"; break;
		case sf::Keyboard::Q: return "Q"; break;
		case sf::Keyboard::R: return "R"; break;
		case sf::Keyboard::S: return "S"; break;
		case sf::Keyboard::T: return "T"; break;
		case sf::Keyboard::U: return "U"; break;
		case sf::Keyboard::V: return "V"; break;
		case sf::Keyboard::W: return "W"; break;
		case sf::Keyboard::X: return "X"; break;
		case sf::Keyboard::Y: return "Y"; break;
		case sf::Keyboard::Z: return "Z"; break;
		case sf::Keyboard::Num0: return "Num0"; break;
		case sf::Keyboard::Num1: return "Num1"; break;
		case sf::Keyboard::Num2: return "Num2"; break;
		case sf::Keyboard::Num3: return "Num3"; break;
		case sf::Keyboard::Num4: return "Num4"; break;
		case sf::Keyboard::Num5: return "Num5"; break;
		case sf::Keyboard::Num6: return "Num6"; break;
		case sf::Keyboard::Num7: return "Num7"; break;
		case sf::Keyboard::Num8: return "Num8"; break;
		case sf::Keyboard::Num9: return "Num9"; break;
		case sf::Keyboard::Escape: return "Escape"; break;
		case sf::Keyboard::LControl: return "LControl"; break;
		case sf::Keyboard::LShift: return "LShift"; break;
		case sf::Keyboard::LAlt: return "LAlt"; break;
		case sf::Keyboard::LSystem: return "LSystem"; break;
		case sf::Keyboard::RControl: return "RControl"; break;
		case sf::Keyboard::RShift: return "RShift"; break;
		case sf::Keyboard::RAlt: return "RAlt"; break;
		case sf::Keyboard::RSystem: return "RSystem"; break;
		case sf::Keyboard::Menu: return "Menu"; break;
		case sf::Keyboard::LBracket: return "LBracket"; break;
		case sf::Keyboard::RBracket: return "RBracket"; break;
		case sf::Keyboard::SemiColon: return "SemiColon"; break;
		case sf::Keyboard::Comma: return "Comma"; break;
		case sf::Keyboard::Period: return "Period"; break;
		case sf::Keyboard::Quote: return "Quote"; break;
		case sf::Keyboard::Slash: return "Slash"; break;
		case sf::Keyboard::BackSlash: return "BackSlash"; break;
		case sf::Keyboard::Tilde: return "Tilde"; break;
		case sf::Keyboard::Equal: return "Equal"; break;
		case sf::Keyboard::Dash: return "Dash"; break;
		case sf::Keyboard::Space: return "Space"; break;
		case sf::Keyboard::Return: return "Return"; break;
		case sf::Keyboard::BackSpace: return "BackSpace"; break;
		case sf::Keyboard::Tab: return "Tab"; break;
		case sf::Keyboard::PageUp: return "PageUp"; break;
		case sf::Keyboard::PageDown: return "PageDown"; break;
		case sf::Keyboard::End: return "End"; break;
		case sf::Keyboard::Home: return "Home"; break;
		case sf::Keyboard::Insert: return "Insert"; break;
		case sf::Keyboard::Delete: return "Delete"; break;
		case sf::Keyboard::Add: return "Add"; break;
		case sf::Keyboard::Subtract: return "Subtract"; break;
		case sf::Keyboard::Multiply: return "Multiply"; break;
		case sf::Keyboard::Divide: return "Divide"; break;
		case sf::Keyboard::Left: return "Left"; break;
		case sf::Keyboard::Right: return "Right"; break;
		case sf::Keyboard::Up: return "Up"; break;
		case sf::Keyboard::Down: return "Down"; break;
		case sf::Keyboard::Numpad0: return "Numpad0"; break;
		case sf::Keyboard::Numpad1: return "Numpad1"; break;
		case sf::Keyboard::Numpad2: return "Numpad2"; break;
		case sf::Keyboard::Numpad3: return "Numpad3"; break;
		case sf::Keyboard::Numpad4: return "Numpad4"; break;
		case sf::Keyboard::Numpad5: return "Numpad5"; break;
		case sf::Keyboard::Numpad6: return "Numpad6"; break;
		case sf::Keyboard::Numpad7: return "Numpad7"; break;
		case sf::Keyboard::Numpad8: return "Numpad8"; break;
		case sf::Keyboard::Numpad9: return "Numpad9"; break;
		case sf::Keyboard::F1: return "F1"; break;
		case sf::Keyboard::F2: return "F2"; break;
		case sf::Keyboard::F3: return "F3"; break;
		case sf::Keyboard::F4: return "F4"; break;
		case sf::Keyboard::F5: return "F5"; break;
		case sf::Keyboard::F6: return "F6"; break;
		case sf::Keyboard::F7: return "F7"; break;
		case sf::Keyboard::F8: return "F8"; break;
		case sf::Keyboard::F9: return "F9"; break;
		case sf::Keyboard::F10: return "F10"; break;
		case sf::Keyboard::F11: return "F11"; break;
		case sf::Keyboard::F12: return "F12"; break;
		case sf::Keyboard::F13: return "F13"; break;
		case sf::Keyboard::F14: return "F14"; break;
		case sf::Keyboard::F15: return "F15"; break;
		case sf::Keyboard::Pause: return "Pause"; break;
		default: return "Unknown"; break;
	}
}

sf::Keyboard::Key InputSystem::stringToKey(std::string const& str)
{
	if (str == "Unknown") { return sf::Keyboard::Unknown; }
	if (str == "A") { return sf::Keyboard::A; }
	if (str == "B") { return sf::Keyboard::B; }
	if (str == "C") { return sf::Keyboard::C; }
	if (str == "D") { return sf::Keyboard::D; }
	if (str == "E") { return sf::Keyboard::E; }
	if (str == "F") { return sf::Keyboard::F; }
	if (str == "G") { return sf::Keyboard::G; }
	if (str == "H") { return sf::Keyboard::H; }
	if (str == "I") { return sf::Keyboard::I; }
	if (str == "J") { return sf::Keyboard::J; }
	if (str == "K") { return sf::Keyboard::K; }
	if (str == "L") { return sf::Keyboard::L; }
	if (str == "M") { return sf::Keyboard::M; }
	if (str == "N") { return sf::Keyboard::N; }
	if (str == "O") { return sf::Keyboard::O; }
	if (str == "P") { return sf::Keyboard::P; }
	if (str == "Q") { return sf::Keyboard::Q; }
	if (str == "R") { return sf::Keyboard::R; }
	if (str == "S") { return sf::Keyboard::S; }
	if (str == "T") { return sf::Keyboard::T; }
	if (str == "U") { return sf::Keyboard::U; }
	if (str == "V") { return sf::Keyboard::V; }
	if (str == "W") { return sf::Keyboard::W; }
	if (str == "X") { return sf::Keyboard::X; }
	if (str == "Y") { return sf::Keyboard::Y; }
	if (str == "Z") { return sf::Keyboard::Z; }
	if (str == "Num0") { return sf::Keyboard::Num0; }
	if (str == "Num1") { return sf::Keyboard::Num1; }
	if (str == "Num2") { return sf::Keyboard::Num2; }
	if (str == "Num3") { return sf::Keyboard::Num3; }
	if (str == "Num4") { return sf::Keyboard::Num4; }
	if (str == "Num5") { return sf::Keyboard::Num5; }
	if (str == "Num6") { return sf::Keyboard::Num6; }
	if (str == "Num7") { return sf::Keyboard::Num7; }
	if (str == "Num8") { return sf::Keyboard::Num8; }
	if (str == "Num9") { return sf::Keyboard::Num9; }
	if (str == "Escape") { return sf::Keyboard::Escape; }
	if (str == "LControl") { return sf::Keyboard::LControl; }
	if (str == "LShift") { return sf::Keyboard::LShift; }
	if (str == "LAlt") { return sf::Keyboard::LAlt; }
	if (str == "LSystem") { return sf::Keyboard::LSystem; }
	if (str == "RControl") { return sf::Keyboard::RControl; }
	if (str == "RShift") { return sf::Keyboard::RShift; }
	if (str == "RAlt") { return sf::Keyboard::RAlt; }
	if (str == "RSystem") { return sf::Keyboard::RSystem; }
	if (str == "Menu") { return sf::Keyboard::Menu; }
	if (str == "LBracket") { return sf::Keyboard::LBracket; }
	if (str == "RBracket") { return sf::Keyboard::RBracket; }
	if (str == "SemiColon") { return sf::Keyboard::SemiColon; }
	if (str == "Comma") { return sf::Keyboard::Comma; }
	if (str == "Period") { return sf::Keyboard::Period; }
	if (str == "Quote") { return sf::Keyboard::Quote; }
	if (str == "Slash") { return sf::Keyboard::Slash; }
	if (str == "BackSlash") { return sf::Keyboard::BackSlash; }
	if (str == "Tilde") { return sf::Keyboard::Tilde; }
	if (str == "Equal") { return sf::Keyboard::Equal; }
	if (str == "Dash") { return sf::Keyboard::Dash; }
	if (str == "Space") { return sf::Keyboard::Space; }
	if (str == "Return") { return sf::Keyboard::Return; }
	if (str == "BackSpace") { return sf::Keyboard::BackSpace; }
	if (str == "Tab") { return sf::Keyboard::Tab; }
	if (str == "PageUp") { return sf::Keyboard::PageUp; }
	if (str == "PageDown") { return sf::Keyboard::PageDown; }
	if (str == "End") { return sf::Keyboard::End; }
	if (str == "Home") { return sf::Keyboard::Home; }
	if (str == "Insert") { return sf::Keyboard::Insert; }
	if (str == "Delete") { return sf::Keyboard::Delete; }
	if (str == "Add") { return sf::Keyboard::Add; }
	if (str == "Subtract") { return sf::Keyboard::Subtract; }
	if (str == "Multiply") { return sf::Keyboard::Multiply; }
	if (str == "Divide") { return sf::Keyboard::Divide; }
	if (str == "Left") { return sf::Keyboard::Left; }
	if (str == "Right") { return sf::Keyboard::Right; }
	if (str == "Up") { return sf::Keyboard::Up; }
	if (str == "Down") { return sf::Keyboard::Down; }
	if (str == "Numpad0") { return sf::Keyboard::Numpad0; }
	if (str == "Numpad1") { return sf::Keyboard::Numpad1; }
	if (str == "Numpad2") { return sf::Keyboard::Numpad2; }
	if (str == "Numpad3") { return sf::Keyboard::Numpad3; }
	if (str == "Numpad4") { return sf::Keyboard::Numpad4; }
	if (str == "Numpad5") { return sf::Keyboard::Numpad5; }
	if (str == "Numpad6") { return sf::Keyboard::Numpad6; }
	if (str == "Numpad7") { return sf::Keyboard::Numpad7; }
	if (str == "Numpad8") { return sf::Keyboard::Numpad8; }
	if (str == "Numpad9") { return sf::Keyboard::Numpad9; }
	if (str == "F1") { return sf::Keyboard::F1; }
	if (str == "F2") { return sf::Keyboard::F2; }
	if (str == "F3") { return sf::Keyboard::F3; }
	if (str == "F4") { return sf::Keyboard::F4; }
	if (str == "F5") { return sf::Keyboard::F5; }
	if (str == "F6") { return sf::Keyboard::F6; }
	if (str == "F7") { return sf::Keyboard::F7; }
	if (str == "F8") { return sf::Keyboard::F8; }
	if (str == "F9") { return sf::Keyboard::F9; }
	if (str == "F10") { return sf::Keyboard::F10; }
	if (str == "F11") { return sf::Keyboard::F11; }
	if (str == "F12") { return sf::Keyboard::F12; }
	if (str == "F13") { return sf::Keyboard::F13; }
	if (str == "F14") { return sf::Keyboard::F14; }
	if (str == "F15") { return sf::Keyboard::F15; }
	if (str == "Pause") { return sf::Keyboard::Pause; }
	return sf::Keyboard::Unknown;
}

std::string InputSystem::buttonToString(sf::Mouse::Button const& button)
{
	switch (button)
	{
		case sf::Mouse::Left: return "Left"; break;
		case sf::Mouse::Right: return "Right"; break;
		case sf::Mouse::Middle: return "Middle"; break;
		case sf::Mouse::XButton1: return "XButton1"; break;
		case sf::Mouse::XButton2: return "XButton2"; break;
		default: return "Left"; break;
	}
}

sf::Mouse::Button InputSystem::stringToButton(std::string const& button)
{
	if (button == "Left") { return sf::Mouse::Left; }
	if (button == "Right") { return sf::Mouse::Right; }
	if (button == "Middle") { return sf::Mouse::Middle; }
	if (button == "XButton1") { return sf::Mouse::XButton1; }
	if (button == "XButton2") { return sf::Mouse::XButton2; }
	return sf::Mouse::Left;
}

std::string InputSystem::unicodeToString(sf::Uint32 unicode)
{
	std::string s;
	s.push_back(static_cast<char>(unicode));
	return s;
}

} // namespace ke