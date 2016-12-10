#include "PropertiesHolder.hpp"

namespace ke
{

PropertiesHolder::PropertiesHolder()
{
}

std::string PropertiesHolder::getProperty(const std::string& id)
{
	return (*this)[id].as<std::string>();
}

void PropertiesHolder::loadFromXml(const pugi::xml_node& node)
{
	pugi::xml_node prop = node.child("properties");
	if (prop)
	{
		for (const pugi::xml_node& property : prop.children("property"))
		{
			std::string name = property.attribute("name").as_string();
			std::string value = property.attribute("value").as_string();
			setProperty(name, value);
		}
	}
}

void PropertiesHolder::saveToXml(pugi::xml_node& node)
{
	std::size_t size = mProperties.size();
	if (size > 0)
	{
		pugi::xml_node properties = node.append_child("properties");
		for (std::size_t i = 0; i < size; i++)
		{
			pugi::xml_node property = properties.append_child("property");
			property.append_attribute("name") = mProperties[i].first.c_str();
			property.append_attribute("value") = mProperties[i].second.c_str();
		}
	}
}

bool PropertiesHolder::loadFromIni(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file)
	{
		return false;
	}
	auto trim = [](std::string line)
	{
		while (line.size() && (line.back() == '\t' || line.back() == ' ')) line.pop_back();
		while (line.size() && (line.front() == '\t' || line.front() == ' ')) line = line.substr(1);
		return line;
	};
	std::string line;
	while (std::getline(file, line))
	{
		line = trim(line);
		if (line.size() >= 1 && line.front() != ';')
		{
			std::size_t found = line.find_first_of('=');
			std::string key = trim(line.substr(0, found));
			std::string value = (found == std::string::npos) ? std::string() : trim(line.substr(found + 1));
			mProperties.push_back(std::pair<std::string, Variant>(key, value));
		}
	}
	return true;
}

bool PropertiesHolder::saveToIni(const std::string& filename)
{
	std::ofstream file(filename);
	if (!file)
	{
		return false;
	}
	for (std::size_t i = 0; i < mProperties.size(); i++)
	{
		file << mProperties[i].first << "=" << mProperties[i].second << std::endl;
	}
	file.close();
	return true;
}

bool PropertiesHolder::propertyExist(const std::string& id) const
{
	std::size_t size = mProperties.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (mProperties[i].first == id)
		{
			return true;
		}
	}
	return false;
}

void PropertiesHolder::removeProperty(const std::string& id)
{
	std::size_t size = mProperties.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (mProperties[i].first == id)
		{
			mProperties.erase(mProperties.begin() + i);
			i--;
		}
	}
}

} // namespace ke
