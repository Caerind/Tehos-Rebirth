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

        bool handleEvent(const sf::Event& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

    private:

};

#endif // PREGAMESTATE_HPP