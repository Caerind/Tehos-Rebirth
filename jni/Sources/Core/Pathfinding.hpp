#ifndef KE_PATHFINDING_HPP
#define KE_PATHFINDING_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Map.hpp"

namespace ke
{

namespace priv
{

struct Node
{
	Node(const sf::Vector2i& c, const sf::Vector2i& p, float co) 
	{ 
		coords = c; 
		parent = p; 
		cost = co;
	}
	sf::Vector2i coords;
	sf::Vector2i parent;
	float cost;
};

class PathFinder
{
	public:
		typedef std::shared_ptr<PathFinder> Ptr;

		PathFinder(unsigned int id, const sf::Vector2i& begin, const sf::Vector2i& end);

		void run();

		unsigned int getId() const;
		bool isFinished() const;
		std::vector<sf::Vector2i> getPath() const;

	private:
		unsigned int mId;
		sf::Vector2i mBegin;
		sf::Vector2i mEnd;
		sf::Thread mThread;
		sf::Mutex mMutex;
		std::vector<sf::Vector2i> mPath;
		bool mFinished;
};

} // namespace priv

class PathFinding
{
	public:
		static unsigned int startPath(const sf::Vector2i& begin, const sf::Vector2i& end);
		static bool hasPath(unsigned int id);
		static bool calculatedPath(unsigned int id);
		static std::vector<sf::Vector2i> getPath(unsigned int id);

		static void setChecker(std::function<bool(const sf::Vector2i& coords)> checker);
		static void setOrientation(const std::string& orientation);
		static void setDiag(bool diag);
		static void setStaggerAxis(const std::string& staggerAxis);
		static void setStaggerIndex(const std::string& staggerIndex);

		static std::vector<sf::Vector2i> pathfinding(const sf::Vector2i& begin, const sf::Vector2i& end);

	private:
		static std::vector<sf::Vector2i> buildPath(priv::Node node, std::vector<priv::Node>& explored);
		static bool coordsValid(const sf::Vector2i& coords);
		static bool coordsExplored(const sf::Vector2i& coords, std::vector<priv::Node>& explored);
		static bool coordsReachable(const sf::Vector2i& coords, std::vector<priv::Node>& reachable);

	private:
		static std::vector<priv::PathFinder::Ptr> mPathFinders;
		static unsigned int mPathFindersIdCounter;

		static std::function<bool(const sf::Vector2i& coords)> mChecker;
		static std::string mOrientation;
		static bool mDiag;
		static std::string mStaggerAxis;
		static std::string mStaggerIndex;


};

} // namespace ke

#endif // KE_PATHFINDING_HPP