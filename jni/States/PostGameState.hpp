#ifndef POSTGAMESTATE_HPP
#define POSTGAMESTATE_HPP

#include "../Sources/System/StateManager.hpp"

class PostGameState : public ke::State
{
    public:
        typedef std::shared_ptr<PostGameState> Ptr;

    public:
        PostGameState();
        virtual ~PostGameState();

        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time dt);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

        virtual void onActivate();
        virtual void onDeactivate();

    private:

};

#endif // POSTGAMESTATE_HPP
