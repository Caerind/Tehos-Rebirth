#ifndef KE_PROPERTIESHOLDER_HPP
#define KE_PROPERTIESHOLDER_HPP

#include <fstream>
#include <string>
#include <unordered_map>

#include "../ExtLibs/pugixml.hpp"

#include "Variant.hpp"

namespace ke
{

class PropertiesHolder
{
    public:
		PropertiesHolder();

		template <typename T> void setProperty(const std::string& id, const T& value)
		{
			std::size_t size = mProperties.size();
			for (std::size_t i = 0; i < size; i++)
			{
				if (mProperties[i].first == id)
				{
					mProperties[i].second = Variant(value);
					return;
				}
			}
			mProperties.push_back(std::pair<std::string, Variant>(id, value));
		}

		Variant& operator[](std::string const& id)
		{
			std::size_t size = mProperties.size();
			for (std::size_t i = 0; i < size; i++)
			{
				if (mProperties[i].first == id)
				{
					return mProperties[i].second;
				}
			}
			mProperties.push_back(std::pair<std::string, Variant>(id, std::string("")));
			return mProperties.back().second;
		}

		std::string getProperty(const std::string& id);
	
		template <typename T> T getPropertyAs(const std::string& id)
		{
			return (*this)[id].as<T>();
		}

		void loadFromXml(const pugi::xml_node& node);
		void saveToXml(pugi::xml_node& node);

		bool loadFromIni(const std::string& filename);
		bool saveToIni(const std::string& filename);

		bool propertyExist(const std::string& id) const;

		void removeProperty(const std::string& id);

    protected:
		std::vector<std::pair<std::string, Variant>> mProperties;
};

} // namespace ke

#endif // KE_PROPERTIESHOLDER_HPP
