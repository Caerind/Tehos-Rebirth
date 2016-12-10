#include "Theme.hpp"

namespace ke
{

Theme::Theme()
	: mTheme(nullptr)
	, mFilename("")
{
}

Theme::Theme(const std::string& filename)
	: mTheme(nullptr)
	, mFilename("")
{
	loadFromFile(filename);
}

Theme::~Theme()
{
}

bool Theme::loadFromFile(const std::string& filename)
{
	mTheme = std::make_shared<tgui::Theme>(filename);
	bool loaded = (mTheme != nullptr);
	setLoaded(loaded);
	mFilename = (loaded) ? filename : "";
	return loaded;
}

tgui::WidgetConverter Theme::create(const std::string& className)
{
	return mTheme->load(className);
}

const std::string& Theme::getFilename() const
{
	return mFilename;
}

void Theme::readFromXml(ResourceManager* manager, const pugi::xml_node& node)
{
	Theme& theme = manager->getResource<Theme>(node.attribute("name").as_string());

	std::string filename = node.attribute("filename").as_string();
	if (filename != "")
	{
		theme.loadFromFile(filename);
	}
}

void Theme::writeToXml(Resource* resource, pugi::xml_node& node)
{
	Theme* theme = dynamic_cast<Theme*>(resource);
	if (theme != nullptr)
	{
		if (theme->getFilename() != "")
		{
			node.append_attribute("filename") = theme->getFilename().c_str();
		}
	}
}

void Theme::registerType(ResourceManager& manager)
{
	manager.setReader(getStaticType(), readFromXml);
	manager.setWriter(getStaticType(), writeToXml);
}

} // namespace ke