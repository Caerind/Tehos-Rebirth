#ifndef KE_CONFIGURATION_HPP
#define KE_CONFIGURATION_HPP

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "ResourceManager.hpp"
#include "Variant.hpp"

namespace ke
{

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

		template <typename T>
		void setProperty(const std::string& id, const T& value);
		std::string getProperty(const std::string& id) const;
		template <typename T>
		T getPropertyAs(const std::string& id) const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		std::string mFilename;
		std::unordered_map<std::string, Variant> mProperties;
};

template<typename T>
inline void Configuration::setProperty(const std::string& id, const T& value)
{
	mProperties[id] = ke::Variant(value);
}

template<typename T>
inline T Configuration::getPropertyAs(const std::string& id) const
{
	auto itr = mProperties.find(id);
	if (itr != mProperties.end())
	{
		return itr->second.as<T>();
	}
	return T();
}

} // namespace ke

#endif // KE_CONFIGURATION_HPP
