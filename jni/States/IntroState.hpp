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

		void loadResources();

		void generateHeroAnimation(const std::string& animation);
		void generateAIAnimation(const std::string& animation, float factor = 1.f);

    private:
		sf::Time mElapsed;
};

#endif // INTROSTATE_HPP
