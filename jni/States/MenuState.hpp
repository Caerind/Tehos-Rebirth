#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "../Sources/System/StateManager.hpp"

class MenuState : public ke::State
{
    public:
        typedef std::shared_ptr<MenuState> Ptr;

    public:
        MenuState();
        virtual ~MenuState();

        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time dt);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

    private:

};

#endif // MENUSTATE_HPP

