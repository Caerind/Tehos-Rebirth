#ifndef KE_THEME_HPP
#define KE_THEME_HPP

#include <memory>

#include <TGUI/Loading/Theme.hpp>

#include "ResourceManager.hpp"

namespace ke
{

class Theme : public Resource
{
	public:
		Theme();
		Theme(const std::string& filename);
		virtual ~Theme();

		TYPE(Theme)

		bool loadFromFile(const std::string& filename);

		tgui::WidgetConverter create(const std::string& className);

		const std::string& getFilename() const;

		static void readFromXml(ResourceManager* manager, const pugi::xml_node& node);
		static void writeToXml(Resource* resource, pugi::xml_node& node);
		static void registerType(ResourceManager& manager);

	private:
		tgui::Theme::Ptr mTheme;
		std::string mFilename;
};

} // namespace ke

#endif // KE_THEME_HPP
