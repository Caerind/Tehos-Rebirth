#ifndef PREGAMESTATE_HPP
#define PREGAMESTATE_HPP

#include "../Sources/System/StateManager.hpp"

class PreGameState : public ke::State
{
    public:
        typedef std::shared_ptr<PreGameState> Ptr;

    public:
        PreGameState();
        virtual ~PreGameState();

        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time dt);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

        virtual void onActivate();
        virtual void onDeactivate();

    private:

};

#endif // PREGAMESTATE_HPP