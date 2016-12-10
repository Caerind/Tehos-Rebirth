#ifndef KE_TIMESYSTEM_HPP
#define KE_TIMESYSTEM_HPP

#include <functional>
#include <map>

#include <SFML/System/Time.hpp>

namespace ke
{

class TimeSystem
{
	public:
		TimeSystem();

		void setTimeFactor(float factor);
		float getTimeFactor() const;

		void update(sf::Time dt);

		std::size_t setTimer(sf::Time duration, std::function<void()> callback = [](){}, bool repeat = false);
		sf::Time getTimerElapsed(std::size_t handle) const;
		sf::Time getTimerRemaining(std::size_t handle) const;
		sf::Time getTimerDuration(std::size_t handle) const;
		float getTimerPercent(std::size_t handle) const;
		void playTimer(std::size_t handle);
		void pauseTimer(std::size_t handle);
		void stopTimer(std::size_t handle);
		void resetTimer(std::size_t handle);
		void restartTimer(std::size_t handle, sf::Time newDuration);
		void repeatTimer(std::size_t handle, bool repeat);

	protected:
		struct Timer
		{
			std::size_t handle;
			sf::Time duration;
			sf::Time elapsed;
			bool playing;
			bool repeat;
			std::function<void()> callback;
		};

	protected:
		float mFactor;
		std::map<std::size_t, Timer> mTimers;
		std::size_t mHandleCounter;
};

} // namespace ke

#endif // KE_TIMESYSTEM_HPP
