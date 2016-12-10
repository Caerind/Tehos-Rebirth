#include "ResourceManager.hpp"

namespace ke
{

Resource::Resource()
	: mName("")
	, mLoaded(false)
{
}

Resource::~Resource()
{
}

std::string Resource::getStaticType()
{
	return "Resource";
}

void Resource::setName(const std::string& name)
{
	mName = name;
}

const std::string& Resource::getName() const
{
	return mName;
}

void Resource::setLoaded(bool loaded)
{
	mLoaded = loaded;
}

bool Resource::isLoaded() const
{
	return mLoaded;
}

ResourceManager::ResourceManager()
	: mResources()
	, mReaders()
	, mWriters()
	, mLogWarning([](const std::string& message) { std::cout << message << std::endl; })
{
}

ResourceManager::~ResourceManager()
{
	releaseAllResources();
}

bool ResourceManager::hasResource(const std::string& name, const std::string& type) const
{
	return std::find_if(mResources.begin(), mResources.end(), [&name, &type](Resource* r) { return (r->getName() == name && (type == "" || r->getType() == type)); }) != mResources.end();
}

bool ResourceManager::isResourceLoaded(const std::string& name, const std::string& type) const
{
	return std::find_if(mResources.begin(), mResources.end(), [&name, &type](Resource* r) { return (r->getName() == name && r->isLoaded() && (type == "" || r->getType() == type)); }) != mResources.end();
}

void ResourceManager::releaseResource(const std::string& name, const std::string& type)
{
	std::size_t size = mResources.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (mResources[i]->getName() == name && (type == "" || mResources[i]->getType() == type))
		{
			delete mResources[i];
			mResources[i] = nullptr;
			mResources.erase(i + mResources.begin());
			size--;
			i--;
		}
	}
}

void ResourceManager::releaseAllResources(const std::string& type)
{
	std::size_t size = mResources.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (type == "" || mResources[i]->getType() == type)
		{
			delete mResources[i];
			mResources[i] = nullptr;
			mResources.erase(i + mResources.begin());
			size--;
			i--;
		}
	}
}

std::size_t ResourceManager::countResources(const std::string& type) const
{
	if (type == "")
	{
		return mResources.size();
	}
	else
	{
		std::size_t count = 0;
		for (Resource* resource : mResources)
		{
			if (resource->getType() == type)
			{
				count++;
			}
		}
		return count;
	}
}

bool ResourceManager::loadFromXml(const std::string& filename)
{
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str()))
	{
		return false;
	}

	pugi::xml_node root = doc.child("Resources");

	for (pugi::xml_node node : root.children())
	{
		bool found = false;
		for (auto itr = mReaders.begin(); itr != mReaders.end() && !found; ++itr)
		{
			if (std::string(node.name()) == itr->first && itr->second)
			{
				found = true;
				itr->second(this, node);
			}
		}
		if (!found && mLogWarning)
		{
			mLogWarning("ResourceReader not found : " + std::string(node.name()));
		}
	}

	return true;
}

bool ResourceManager::loadFromMemory(const std::string& memory)
{
	pugi::xml_document doc;
	if (!doc.load(memory.c_str()))
	{
		return false;
	}

	pugi::xml_node root = doc.child("Resources");

	for (pugi::xml_node node : root.children())
	{
		bool found = false;
		for (auto itr = mReaders.begin(); itr != mReaders.end() && !found; ++itr)
		{
			if (std::string(node.name()) == itr->first && itr->second)
			{
				found = true;
				itr->second(this, node);
			}
		}
		if (!found && mLogWarning)
		{
			mLogWarning("ResourceReader not found : " + std::string(node.name()));
		}
	}

	return true;
}

bool ResourceManager::saveToXml(const std::string& filename)
{
	pugi::xml_document doc;

	pugi::xml_node root = doc.append_child("Resources");

	for (Resource* resource : mResources)
	{
		bool found = false;
		for (auto itr = mWriters.begin(); itr != mWriters.end() && !found; ++itr)
		{
			if (resource->getType() == itr->first && itr->second)
			{
				found = true;
				pugi::xml_node node = root.append_child(resource->getType().c_str());
				node.append_attribute("name") = resource->getName().c_str();
				itr->second(resource, node);
			}
		}
	}

	doc.save_file(filename.c_str());

	return true;
}

void ResourceManager::setReader(const std::string& type, ResourceReader reader)
{
	mReaders[type] = reader;
}

void ResourceManager::setWriter(const std::string& type, ResourceWriter writer)
{
	mWriters[type] = writer;
}

void ResourceManager::setLogWarning(std::function<void(const std::string&)> log)
{
	mLogWarning = log;
}

} // namespace ke