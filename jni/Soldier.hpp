#ifndef SOLDIER_HPP
#define SOLDIER_HPP

#include "AI.hpp"

class Soldier : public AI
{
	public:
		typedef std::shared_ptr<Soldier> Ptr;

		Soldier(ke::Scene& scene, std::size_t type);
		~Soldier();

		std::size_t getTeam() const;

		bool isGiant() const;

	private:
		void loadData();
};

#endif // SOLDIER_HPP
