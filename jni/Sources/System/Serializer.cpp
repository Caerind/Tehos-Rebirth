#include "Serializer.hpp"

namespace ke
{

Serializer::Serializer()
{
}

Serializer::~Serializer()
{
}

bool Serializer::openDocument(const std::string& filename, bool erase, const std::string& root)
{
	bool ret = true;
	mFilename = filename;
	if (erase)
	{
		mDoc.append_child(root.c_str());
		saveDocument();
	}
	else
	{
		std::ifstream file(filename);
		if (file)
		{
			mDoc.load(file);
			if (!mDoc.child(root.c_str()))
			{
				mDoc.append_child(root.c_str());
			}
		}
		else
		{
			mDoc.append_child(root.c_str());
			ret = false;
		}
		file.close();
	}

	mRootNode = mDoc.child(root.c_str());
	mActualNode = mRootNode;
	return ret;
}

void Serializer::saveDocument(bool readable)
{
	if (readable)
	{
		mDoc.save_file(mFilename.c_str());
	}
	else
	{
		mDoc.save_file(mFilename.c_str(), "");
	}
}

pugi::xml_node& Serializer::getRootNode()
{
	return mRootNode;
}

void Serializer::setNode(const pugi::xml_node& node)
{
	mActualNode = node;
}

pugi::xml_node& Serializer::getNode()
{
	return mActualNode;
}

void Serializer::create(const std::string& identifier)
{
	mActualNode = mActualNode.append_child(identifier.c_str());
}

void Serializer::close()
{
	mActualNode = mActualNode.parent();
}

bool Serializer::read(const std::string& identifier)
{
	if (mActualNode.child(identifier.c_str()))
	{
		mActualNode = mActualNode.child(identifier.c_str());
		return true;
	}
	return false;
}

void Serializer::end()
{
	pugi::xml_node rem = mActualNode;
	mActualNode = mActualNode.parent();
	mActualNode.remove_child(rem);
}

} // namespace ke