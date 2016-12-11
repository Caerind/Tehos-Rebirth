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

        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time dt);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

        virtual void onActivate();
        virtual void onDeactivate();

    private:

};

#endif // SETTINGSTATE_HPP