#ifndef POPUP_HPP
#define POPUP_HPP

#include "../Sources/System/StateManager.hpp"

class PopUp : public ke::State
{
    public:
        typedef std::shared_ptr<PopUp> Ptr;

    public:
        PopUp();
        virtual ~PopUp();

        bool handleEvent(const sf::Event& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
		
		static int Ref;

		void okDouble();
		void ok();

    private:
		sf::Sprite mWindow;
		sf::RectangleShape mMask;
		sf::Text mText;
		
		int mPrice;
		int mCrystals;
		bool mDouble;
};

#endif // POPUP_HPP