#include "Configuration.hpp"

namespace ke
{
	
Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

bool Configuration::loadFromFile(const std::string& filename)
{
	mFilename = filename;
	std::ifstream file(filename);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			if (line != "")
			{
				while (line[0] == ' ')
				{
					line.erase(line.begin());
				}
				if (line[0] != '#')
				{
					std::size_t found = line.find_first_of(" = ");
					if (found != std::string::npos)
					{
						mProperties.push_back(std::pair<std::string, Variant>(line.substr(0, found), line.substr(found + 3)));
					}
				}
			}
		}
		file.close();
		setLoaded(false);
		return true;
	}
	else
	{
		setLoaded(false);
		return false;
	}
}

bool Configuration::loadFromMemory(const std::string& memory)
{
	std::string line;
	std::stringstream ss(memory);
	while (std::getline(ss, line))
	{
		if (line != "")
		{
			while (line[0] == ' ')
			{
				line.erase(line.begin());
			}
			if (line[0] != '#')
			{
				std::size_t found = line.find_first_of(" = ");
				if (found != std::string::npos)
				{
					mProperties.push_back(std::pair<std::string, Variant>(line.substr(0, found), line.substr(found + 3)));
				}
			}
		}
	}
	setLoaded(true);
	mFilename = "";
	return true;
}

bool Configuration::saveToFile(const std::string& filename)
{
	if (filename == "" && mFilename == "")
	{
		return false;
	}
	else if (filename == "")
	{
		saveToFile(mFilename);
	}
	std::ofstream file(filename);
	if (file.is_open())
	{
		for (std::size_t i = 0; i < mProperties.size(); i++)
		{
			file << mProperties[i].first << " = " << mProperties[i].second << std::endl;
		}
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

const std::string& Configuration::getFilename()
{
	return mFilename;
}

std::string Configuration::getProperty(const std::string& id) const
{
	for (std::size_t i = 0; i < mProperties.size(); i++)
	{
		if (mProperties[i].first == id)
		{
			return mProperties[i].second;
		}
	}
	return "";
}

void Configuration::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	Configuration& configuration = manager->getResource<Configuration>(node.attribute("name").as_string());

	std::string filename = node.attribute("filename").as_string();
	if (filename != "")
	{
		configuration.loadFromFile(filename);
	}
}

void Configuration::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Configuration* configuration = dynamic_cast<Configuration*>(resource);
	if (configuration != nullptr)
	{
		if (configuration->getFilename() != "")
		{
			node.append_attribute("filename") = configuration->getFilename().c_str();
		}
	}
}

void Configuration::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

} // namespace ke
