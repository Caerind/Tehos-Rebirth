#include "ParallelTask.hpp"

namespace ke
{

ParallelTask::ParallelTask()
: mThread(&ParallelTask::runTask, this)
, mFinished(false)
{
}

void ParallelTask::execute()
{
	mFinished = false;
	mElapsedTime.restart();
	mThread.launch();
}

bool ParallelTask::isFinished()
{
	sf::Lock lock(mMutex);
	return mFinished;
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(mMutex); // Protect the clock

    //
    // RETURN A PERCENT
    //

	return 0.f;
}

void ParallelTask::runTask()
{
	bool ended = false;
	while (!ended)
	{
		sf::Lock lock(mMutex); // Protect the clock


		//
		// YOUR TASK
		//
	}

	{ // mFinished may be accessed from multiple threads, protect it
		sf::Lock lock(mMutex);
		mFinished = true;
	}
}

} // namespace ke