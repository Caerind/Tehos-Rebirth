#ifndef KE_STRING_HPP
#define KE_STRING_HPP

#include <iostream>
#include <algorithm>
#include <string>
#include <typeinfo>
#include <vector>
#include <sstream>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace ke
{

void toLower(std::string& str);
void toUpper(std::string& str);

std::string split(std::string& base, char separator);
std::string split(std::string& base, std::string const& separator);
std::vector<std::string> splitVector(std::string str, std::string const& separator);

bool contains(std::string const& str, char c);
bool contains(std::string const& str, std::string const& c);

std::string limitSize(std::string str, std::size_t size);

template <typename T>
std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template <> inline std::string toString<std::string>(const std::string& value)
{
	return value;
}

template <> inline std::string toString<bool>(const bool& value)
{
	return (value) ? "true" : "false";
}

template <> inline std::string toString<char>(const char& value)
{
	return std::string(1, value);
}

template <> inline std::string toString<sf::Vector2f>(const sf::Vector2f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y;
	return oss.str();
}

template <> inline std::string toString<sf::Vector2i>(const sf::Vector2i& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y;
	return oss.str();
}

template <> inline std::string toString<sf::Vector2u>(const sf::Vector2u& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y;
	return oss.str();
}

template <> inline std::string toString<sf::Vector3f>(const sf::Vector3f& value)
{
	std::ostringstream oss;
	oss << value.x << "," << value.y << "," << value.z;
	return oss.str();
}

template <> inline std::string toString<sf::Color>(const sf::Color& value)
{
	std::ostringstream oss;
	oss << std::hex << value.toInteger();
	return oss.str();
}

template <> inline std::string toString<sf::IntRect>(const sf::IntRect& value)
{
	std::ostringstream oss;
	oss << value.left << "," << value.top << "," << value.width << "," << value.height;
	return oss.str();
}

template <> inline std::string toString<sf::FloatRect>(const sf::FloatRect& value)
{
	std::ostringstream oss;
	oss << value.left << "," << value.top << "," << value.width << "," << value.height;
	return oss.str();
}

template <> inline std::string toString<sf::Time>(const sf::Time& value)
{
	return toString(value.asSeconds());
}

template <typename T>
T fromString(const std::string& string)
{
	T value;
	std::istringstream iss(string);
	iss >> value;
	return value;
}

template <> inline std::string fromString<std::string>(const std::string& string)
{
	return string;
}

template <> inline bool fromString<bool>(const std::string& string)
{
	if (string == "true")
	{
		return true;
	}
	if (string == "false")
	{
		return false;
	}
	bool value;
	std::istringstream iss(string);
	iss >> value;
	return value;
}

template <> inline char fromString<char>(const std::string& string)
{
	if (string.size() >= 1)
	{
		return string[0];
	}
	return '\0';
}

template <> inline sf::Vector2f fromString<sf::Vector2f>(const std::string& string)
{
	sf::Vector2f vector;
	std::size_t found = string.find_first_of(',');
	if (found != std::string::npos)
	{
		std::istringstream iss(string.substr(0, found));
		iss >> vector.x;
		iss.clear();
		iss.str(string.substr(found + 1));
		iss >> vector.y;
	}
	return vector;
}

template <> inline sf::Vector2i fromString<sf::Vector2i>(const std::string& string)
{
	sf::Vector2i vector;
	std::size_t found = string.find_first_of(',');
	if (found != std::string::npos)
	{
		std::istringstream iss(string.substr(0, found));
		iss >> vector.x;
		iss.clear();
		iss.str(string.substr(found + 1));
		iss >> vector.y;
	}
	return vector;
}

template <> inline sf::Vector2u fromString<sf::Vector2u>(const std::string& string)
{
	sf::Vector2u vector;
	std::size_t found = string.find_first_of(',');
	if (found != std::string::npos)
	{
		std::istringstream iss(string.substr(0, found));
		iss >> vector.x;
		iss.clear();
		iss.str(string.substr(found + 1));
		iss >> vector.y;
	}
	return vector;
}

template <> inline sf::Vector3f fromString<sf::Vector3f>(const std::string& string)
{
	std::string z = string;
	std::string x = split(z, ",");
	std::string y = split(z, ",");
	return sf::Vector3f(fromString<float>(x), fromString<float>(y), fromString<float>(z));
}

template <> inline sf::Color fromString<sf::Color>(const std::string& string)
{
	std::string color = string;
	if (color != "")
	{
		if (color[0] == '#')
		{
			color.erase(color.begin());
		}
		sf::Uint32 hexTrans;
		std::stringstream ss(color);
		ss >> std::hex >> hexTrans;
		if (hexTrans >= 0)
		{
			sf::Uint8 red, green, blue, alpha;
			red = (hexTrans >> 24) & 0xff;
			green = (hexTrans >> 16) & 0xff;
			blue = (hexTrans >> 8) & 0xff;
			alpha = hexTrans & 0xff;
			return sf::Color(red, green, blue, alpha);
		}
	}
	return sf::Color::Transparent;
}

template <> inline sf::IntRect fromString<sf::IntRect>(const std::string& string)
{
	std::string h = string;
	std::string x = split(h, ",");
	std::string y = split(h, ",");
	std::string w = split(h, ",");
	return sf::IntRect(fromString<int>(x), fromString<int>(y), fromString<int>(w), fromString<int>(h));
}

template <> inline sf::FloatRect fromString<sf::FloatRect>(const std::string& string)
{
	std::string h = string;
	std::string x = split(h, ",");
	std::string y = split(h, ",");
	std::string w = split(h, ",");
	return sf::FloatRect(fromString<float>(x), fromString<float>(y), fromString<float>(w), fromString<float>(h));
}

template <> inline sf::Time fromString<sf::Time>(const std::string& string)
{
	return sf::seconds(fromString<float>(string));
}

} // namespace ke

#endif // KE_STRING_HPP
