#include "Pathfinding.hpp"

namespace ke
{

std::vector<priv::PathFinder::Ptr> PathFinding::mPathFinders;
unsigned int PathFinding::mPathFindersIdCounter = 1;

std::function<bool(const sf::Vector2i& coords)> PathFinding::mChecker = [](const sf::Vector2i& coords) -> bool { return true; };
std::string PathFinding::mOrientation = "orthogonal";
bool PathFinding::mDiag = false;
std::string PathFinding::mStaggerAxis = "y";
std::string PathFinding::mStaggerIndex = "odd";

priv::PathFinder::PathFinder(unsigned int id, const sf::Vector2i& begin, const sf::Vector2i& end)
	: mId(id)
	, mBegin(begin)
	, mEnd(end)
	, mThread(&PathFinder::run, this)
	, mFinished(false)
{
	mThread.launch();
}

void priv::PathFinder::run()
{
	mFinished = false;
	mMutex.lock();
	mPath = PathFinding::pathfinding(mBegin, mEnd);
	mMutex.unlock();
	mFinished = true;
}

unsigned int priv::PathFinder::getId() const
{
	return mId;
}

bool priv::PathFinder::isFinished() const
{
	return mFinished;
}

std::vector<sf::Vector2i> priv::PathFinder::getPath() const
{
	if (mFinished)
	{
		return mPath;
	}
	return std::vector<sf::Vector2i>();
}

unsigned int PathFinding::startPath(const sf::Vector2i& begin, const sf::Vector2i& end)
{
	mPathFinders.push_back(std::make_shared<priv::PathFinder>(mPathFindersIdCounter++, begin, end));
	return mPathFinders.back()->getId();
}

bool PathFinding::hasPath(unsigned int id)
{
	for (std::size_t i = 0; i < mPathFinders.size(); i++)
	{
		if (mPathFinders[i]->getId() == id)
		{
			return true;
		}
	}
	return false;
}

bool PathFinding::calculatedPath(unsigned int id)
{
	for (std::size_t i = 0; i < mPathFinders.size(); i++)
	{
		if (mPathFinders[i]->getId() == id)
		{
			if (mPathFinders[i]->isFinished())
			{
				return true;
			}
		}
	}
	return false;
}

std::vector<sf::Vector2i> PathFinding::getPath(unsigned int id)
{
	for (std::size_t i = 0; i < mPathFinders.size(); i++)
	{
		if (mPathFinders[i]->getId() == id)
		{
			if (mPathFinders[i]->isFinished())
			{
				std::vector<sf::Vector2i> p = mPathFinders[i]->getPath();
				mPathFinders.erase(i + mPathFinders.begin());
				i--;
				return p;
			}
		}
	}
	return std::vector<sf::Vector2i>();
}

void PathFinding::setChecker(std::function<bool(const sf::Vector2i& coords)> checker)
{
	mChecker = checker;
}

void PathFinding::setOrientation(const std::string& orientation)
{
	mOrientation = orientation;
}

void PathFinding::setDiag(bool diag)
{
	mDiag = diag;
}

void PathFinding::setStaggerAxis(const std::string& staggerAxis)
{
	mStaggerAxis = staggerAxis;
}

void PathFinding::setStaggerIndex(const std::string& staggerIndex)
{
	mStaggerIndex = staggerIndex;
}

std::vector<sf::Vector2i> PathFinding::pathfinding(const sf::Vector2i& begin, const sf::Vector2i& end)
{
	if (begin == end)
	{
		return std::vector<sf::Vector2i>();
	}
	std::vector<priv::Node> reachable;
	std::vector<priv::Node> explored;
	reachable.push_back(priv::Node(begin, sf::Vector2i(666,666), 0.f));
	while (!reachable.empty())
	{
		std::sort(reachable.begin(), reachable.end(), [](priv::Node& a, priv::Node& b) { return a.cost < b.cost; });

		if (reachable.front().coords == end)
		{
			return buildPath(reachable.front(), explored);
		}

		explored.push_back(reachable.front());
		reachable.erase(reachable.begin());

		std::vector<sf::Vector2i> newr = Map::getNeighboors(explored.back().coords, mOrientation, mDiag, mStaggerIndex, mStaggerAxis);
		std::size_t size = newr.size();
		for (std::size_t i = 0; i < size; i++)
		{
			bool valid = coordsValid(newr[i]);
			if (valid)
			{
				bool explrd = coordsExplored(newr[i], explored);
				if (!explrd)
				{
					bool reached = coordsReachable(newr[i], reachable);
					if (!reached)
					{
						reachable.push_back(priv::Node(newr[i], explored.back().coords, explored.back().cost + 1.f));
					}
				}
			}
		}
	}
	return std::vector<sf::Vector2i>();
}

std::vector<sf::Vector2i> PathFinding::buildPath(priv::Node node, std::vector<priv::Node>& explored)
{
	std::vector<sf::Vector2i> path;
	bool parentFound;
	do
	{
		parentFound = false;
		for (std::size_t i = 0; !parentFound && i < explored.size(); i++)
		{
			if (node.parent == explored[i].coords)
			{
				parentFound = true;
				path.push_back(node.coords);
				node = explored[i];
			}
		}
	} while (parentFound);
	path.push_back(node.coords);
	std::reverse(path.begin(), path.end());
	return path;
}

bool PathFinding::coordsValid(const sf::Vector2i& coords)
{
	if (mChecker)
	{
		return mChecker(coords);
	}
	return true;
}

bool PathFinding::coordsExplored(const sf::Vector2i& coords, std::vector<priv::Node>& explored)
{
	std::size_t size = explored.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (explored[i].coords == coords)
		{
			return true;
		}
	}
	return false;
}

bool PathFinding::coordsReachable(const sf::Vector2i& coords, std::vector<priv::Node>& reachable)
{
	std::size_t size = reachable.size();
	for (std::size_t i = 0; i < size; i++)
	{
		if (reachable[i].coords == coords)
		{
			return true;
		}
	}
	return false;
}

} // namespace ke
