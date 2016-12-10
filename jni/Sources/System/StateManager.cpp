#include "StateManager.hpp"
#include "Application.hpp"

namespace ke
{

State::State() : mGui(Application::getWindow().getHandle())
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

void State::onActivate()
{
}

void State::onDeactivate()
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
	for (auto itr = mStates.begin(); itr != mStates.end(); itr++)
	{
		(*itr)->onDeactivate();
	}
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

std::vector<std::string> StateManager::getStateOrder() const
{
	return mStateOrder;
}

std::string StateManager::getActualState() const
{
	if (mStateOrder.empty())
	{
		return "";
	}
	return mStateOrder.back();
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
			if (!mStates.empty())
			{
				mStates.back()->onDeactivate();
			}
			mStates.push_back(createState(change.id));
			mStateOrder.push_back(change.id);
			mStates.back()->onActivate();
			break;

		case Action::Pop:
			if (!mStates.empty())
			{
				mStates.back()->onDeactivate();
				mStates.pop_back();
				mStateOrder.pop_back();
			}
			if (!mStates.empty())
			{
				mStates.back()->onActivate();
			}
			break;

		case Action::Clear:
			if (!mStates.empty())
			{
				mStates.back()->onDeactivate();
			}
			mStates.clear();
			mStateOrder.clear();
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