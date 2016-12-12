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

		void updatePage();

    private:
		sf::Sprite mWindow;
		sf::RectangleShape mMask;
		sf::FloatRect mCloseButton;
		sf::FloatRect mLeftArrow;
		sf::FloatRect mRightArrow;
		int mPage;
		int mMaxLevel;
		std::size_t mButtonsPerPage;
		std::vector<sf::Text> mTexts;
};

#endif // PREGAMESTATE_HPP