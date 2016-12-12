#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "../Sources/System/StateManager.hpp"

class SettingsState : public ke::State
{
    public:
        typedef std::shared_ptr<SettingsState> Ptr;

    public:
        SettingsState();
        virtual ~SettingsState();

        bool handleEvent(const sf::Event& event);
		bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

		static sf::FloatRect CloseButton;

    private:
		sf::Sprite mWindow;
		sf::RectangleShape mButton;
};

#endif // SETTINGSTATE_HPP