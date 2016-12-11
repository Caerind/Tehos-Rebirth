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
		std::vector<std::pair<std::string, Variant>> mProperties;
};

template<typename T>
inline void Configuration::setProperty(const std::string& id, const T& value)
{
	bool found = false;
	for (std::size_t i = 0; i < mProperties.size(); i++)
	{
		if (!found && mProperties[i].first == id)
		{
			mProperties[i].second = Variant(value);
			found = true;
		}
	}
	if (!found)
	{
		mProperties.push_back(std::pair<std::string, Variant>(id, Variant(value)));
	}
}

template<typename T>
inline T Configuration::getPropertyAs(const std::string& id) const
{
	for (std::size_t i = 0; i < mProperties.size(); i++)
	{
		if (mProperties[i].first == id)
		{
			return mProperties[i].second.as<T>();
		}
	}
	return T();
}

} // namespace ke

#endif // KE_CONFIGURATION_HPP
