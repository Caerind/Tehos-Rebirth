#ifndef KE_CONFIGURATION_HPP
#define KE_CONFIGURATION_HPP

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "ResourceManager.hpp"

namespace ke
{

// TODO : Maybe use Variant ?

class Configuration : public Resource
{
	public:
		Configuration();
		virtual ~Configuration();

		TYPE(Configuration)

		bool loadFromFile(const std::string& filename);
		bool loadFromMemory(const std::string& memory);

		bool saveToFile(const std::string& filename = "");

		const std::string& getFilename();

		void setProperty(const std::string& id, const std::string& value);
		std::string getProperty(const std::string& id) const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		std::string mFilename;
		std::unordered_map<std::string, std::string> mProperties;
};

} // namespace ke

#endif // KE_CONFIGURATION_HPP
