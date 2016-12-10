#ifndef KE_STATEMANAGER_HPP
#define KE_STATEMANAGER_HPP

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <TGUI/TGUI.hpp>

namespace ke
{

class Application;
class State
{
    public:
        typedef std::shared_ptr<State> Ptr;

    public:
		State();

		virtual ~State();

		virtual bool handleEvent(sf::Event const& event);

		virtual bool update(sf::Time dt);

		virtual void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

		virtual void onActivate();

		virtual void onDeactivate();

		Application& getApplication();

		void popState();
		void pushState(std::string const& state);
		void clearStates();

	protected:
		tgui::Gui mGui;
};

class StateManager
{
    public:
		StateManager();

		~StateManager();

		template <typename T>
		void registerState(std::string const& className)
        {
            mFactories[className] = [this]()
            {
                return State::Ptr(new T());
            };
        }

		void handleEvent(sf::Event const& event);

		void update(sf::Time dt);

		void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

		void pushState(std::string const& className);

		void popState();

		void clearStates();

		std::size_t stateCount() const;

		std::vector<std::string> getStateOrder() const;

		std::string getActualState() const;

		template <typename T>
		std::shared_ptr<T> getActualTypedState()
		{
			return std::dynamic_pointer_cast<T>(mStates.back());
		}

	protected:
		enum Action
		{
			Push,
			Pop,
			Clear
		};

		struct PendingChange
		{
			explicit PendingChange(Action action, std::string const& id = "");

			Action action;
			std::string id;
		};

		void applyPendingChanges();

		State::Ptr createState(std::string const& id);

	protected:
		std::vector<State::Ptr> mStates;
		std::vector<std::string> mStateOrder;
		std::vector<PendingChange> mPendingList;

		std::map<std::string, std::function<State::Ptr()>> mFactories;
};

} // namespace ke

#endif // KE_STATEMANAGER_HPP
