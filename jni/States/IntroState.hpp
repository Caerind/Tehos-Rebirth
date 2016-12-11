#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include "../Sources/System/StateManager.hpp"

class IntroState : public ke::State
{
    public:
        typedef std::shared_ptr<IntroState> Ptr;

    public:
        IntroState();
        virtual ~IntroState();

        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time dt);
        virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

		void toMenu();

    private:
		sf::Time mElapsed;
};

#endif // INTROSTATE_HPP
