#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include "../Sources/System/StateManager.hpp"

class IntroState : public ke::State
{
    public:
        typedef std::shared_ptr<IntroState> Ptr;

    public:
        IntroState();

        virtual bool update(sf::Time dt);

    private:
		sf::Time mElapsed;
};

#endif // INTROSTATE_HPP
