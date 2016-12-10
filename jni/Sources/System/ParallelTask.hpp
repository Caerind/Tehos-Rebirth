#ifndef KE_PARALLELTASK_HPP
#define KE_PARALLELTASK_HPP

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>

namespace ke
{

class ParallelTask
{
	public:
        ParallelTask();

		void execute();
		bool isFinished();
		float getCompletion();

    protected:
		void runTask();

	private:
		sf::Thread mThread;
		bool mFinished;
		sf::Clock mElapsedTime;
		sf::Mutex mMutex;
};

} // namespace ke

#endif // KE_PARALLELTASK_HPP
