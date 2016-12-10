#include "TimeSystem.hpp"

namespace ke
{

TimeSystem::TimeSystem()
{
	mFactor = 1.f;
}

void TimeSystem::setTimeFactor(float factor)
{
	mFactor = factor;
}

float TimeSystem::getTimeFactor() const
{
	return mFactor;
}

void TimeSystem::update(sf::Time dt)
{
	for (auto itr = mTimers.begin(); itr != mTimers.end();)
	{
		itr->second.elapsed += dt;
		if (itr->second.playing && itr->second.elapsed >= itr->second.duration)
		{
			if (itr->second.callback)
			{
				itr->second.callback();
			}
			if (itr->second.repeat)
			{
				itr->second.elapsed = sf::Time::Zero;
				itr++;
			}
			else
			{
				itr = mTimers.erase(itr);
			}
		}
	}
}

std::size_t TimeSystem::setTimer(sf::Time duration, std::function<void()> callback, bool repeat)
{
	std::size_t handle = mHandleCounter++;
	mTimers[handle].handle = handle;
	mTimers[handle].duration = duration;
	mTimers[handle].elapsed = sf::Time::Zero;
	mTimers[handle].playing = true;
	mTimers[handle].repeat = repeat;
	mTimers[handle].callback = callback;
	return handle;
}

sf::Time TimeSystem::getTimerElapsed(std::size_t handle) const
{
	if (mTimers.find(handle) != mTimers.end())
	{
		return mTimers.at(handle).elapsed;
	}
	return sf::Time::Zero;
}

sf::Time TimeSystem::getTimerRemaining(std::size_t handle) const
{
	if (mTimers.find(handle) != mTimers.end())
	{
		return mTimers.at(handle).duration - mTimers.at(handle).elapsed;
	}
	return sf::Time::Zero;
}

sf::Time TimeSystem::getTimerDuration(std::size_t handle) const
{
	if (mTimers.find(handle) != mTimers.end())
	{
		return mTimers.at(handle).duration;
	}
	return sf::Time::Zero;
}

float TimeSystem::getTimerPercent(std::size_t handle) const
{
	if (mTimers.find(handle) != mTimers.end())
	{
		return mTimers.at(handle).elapsed.asSeconds() / mTimers.at(handle).duration.asSeconds();
	}
	return 0.f;
}

void TimeSystem::playTimer(std::size_t handle)
{
	if (mTimers.find(handle) != mTimers.end())
	{
		mTimers.at(handle).playing = true;
	}
}

void TimeSystem::pauseTimer(std::size_t handle)
{
	if (mTimers.find(handle) != mTimers.end())
	{
		mTimers.at(handle).playing = false;
	}
}

void TimeSystem::stopTimer(std::size_t handle)
{
	auto itr = mTimers.find(handle);
	if (itr != mTimers.end())
	{
		mTimers.erase(itr);
	}
}

void TimeSystem::resetTimer(std::size_t handle)
{
	if (mTimers.find(handle) != mTimers.end())
	{
		mTimers.at(handle).elapsed = sf::Time::Zero;
	}
}

void TimeSystem::restartTimer(std::size_t handle, sf::Time newDuration)
{
	if (mTimers.find(handle) != mTimers.end())
	{
		mTimers.at(handle).elapsed = sf::Time::Zero;
		mTimers.at(handle).duration = newDuration;
	}
}

void TimeSystem::repeatTimer(std::size_t handle, bool repeat)
{
	if (mTimers.find(handle) != mTimers.end())
	{
		mTimers.at(handle).repeat = repeat;
	}
}

} // namespace ke