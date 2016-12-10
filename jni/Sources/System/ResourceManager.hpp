#ifndef KE_RESOURCEMANAGER_HPP
#define KE_RESOURCEMANAGER_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../ExtLibs/pugixml.hpp"

#include "Type.hpp"

namespace ke
{

class Resource
{
	public:
		Resource();
		virtual ~Resource();

		static std::string getStaticType();
		virtual std::string getType() = 0;

		void setName(const std::string& name);
		const std::string& getName() const;

		void setLoaded(bool loaded);
		bool isLoaded() const;

	protected:
		std::string mName;
		bool mLoaded;
};

class ResourceManager
{
	public:
		ResourceManager();
		~ResourceManager();

		template <typename T, typename ... Args>
		T& getResource(const std::string& name, Args&& ... args);

		bool hasResource(const std::string& name, const std::string& type = "") const;

		bool isResourceLoaded(const std::string& name, const std::string& type = "") const;

		void releaseResource(const std::string& name, const std::string& type = "");

		void releaseAllResources(const std::string& type = "");

		std::size_t countResources(const std::string& type = "") const;

		bool loadFromXml(const std::string& filename);
		bool loadFromMemory(const std::string& memory);
		bool saveToXml(const std::string& filename);

		typedef std::function<void(ResourceManager*, const pugi::xml_node&)> ResourceReader;
		typedef std::function<void(Resource*, pugi::xml_node&)> ResourceWriter;

		void setReader(const std::string& type, ResourceReader reader = [](ResourceManager*, const pugi::xml_node&) {});
		void setWriter(const std::string& type, ResourceWriter writer = [](Resource*, pugi::xml_node&){});

		void setLogWarning(std::function<void(const std::string&)> log);

	private:
		std::vector<Resource*> mResources;

		std::map<std::string, ResourceReader> mReaders;
		std::map<std::string, ResourceWriter> mWriters;

		std::function<void(const std::string&)> mLogWarning;
};

template<typename T, typename ... Args>
inline T& ResourceManager::getResource(const std::string& name, Args&& ... args)
{
	// Check existence
	for (Resource* resource : mResources)
	{
		if (resource->getName() == name && T::getStaticType() == resource->getType())
		{
			return *dynamic_cast<T*>(resource);
		}
	}

	// If don't exists, create it
	mResources.push_back(new T(std::forward<Args>(args)...));
	mResources.back()->setName(name);
	return *dynamic_cast<T*>(mResources.back());
}

} // namespace ke

#endif // KE_RESOURCEMANAGER_HPP