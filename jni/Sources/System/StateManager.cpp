#include "StateManager.hpp"
#include "Application.hpp"

namespace ke
{

State::State()
{
}

State::~State()
{
}

bool State::handleEvent(sf::Event const& event)
{
	return true;
}

bool State::update(sf::Time dt)
{
	return true;
}

void State::render(sf::RenderTarget& target, sf::RenderStates states)
{
}

Application& State::getApplication()
{
	return Application::instance();
}

void State::popState()
{
	Application::popState();
}

void State::pushState(std::string const& state)
{
	Application::pushState(state);
}

void State::clearStates()
{
	Application::clearStates();
}

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
	mStates.clear();
}

void StateManager::handleEvent(sf::Event const& event)
{
	for (auto itr = mStates.rbegin(); itr != mStates.rend(); itr++)
	{
		if (!(*itr)->handleEvent(event))
		{
			break;
		}
	}
	applyPendingChanges();
}

void StateManager::update(sf::Time dt)
{
	for (auto itr = mStates.rbegin(); itr != mStates.rend(); itr++)
	{
		if (!(*itr)->update(dt))
		{
			break;
		}
	}
	applyPendingChanges();
}

void StateManager::render(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto itr = mStates.begin(); itr != mStates.end(); itr++)
	{
		(*itr)->render(target, states);
	}
}

void StateManager::pushState(std::string const& className)
{
	mPendingList.push_back(PendingChange(Push, className));
}

void StateManager::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateManager::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

std::size_t StateManager::stateCount() const
{
	return mStates.size();
}

StateManager::PendingChange::PendingChange(Action action, std::string const& id )
	: action(action)
	, id(id)
{
}

void StateManager::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Action::Push:
			mStates.push_back(createState(change.id));
			break;

		case Action::Pop:
			if (!mStates.empty())
			{
				mStates.pop_back();
			}
			break;

		case Action::Clear:
			mStates.clear();
			break;
		}
	}
	mPendingList.clear();
}

std::shared_ptr<State> StateManager::createState(std::string const& id)
{
	auto found = mFactories.find(id);
	if (found == mFactories.end())
	{
		std::cerr << "State : " << id << " : cant be loaded" << std::endl;
		assert(false);
	}
	return found->second();
}

} // namespace ke