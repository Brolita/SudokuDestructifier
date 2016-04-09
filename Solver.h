#include "Build Settings.h"

#ifndef SOLVER
#define SOLVER

class Mutation {
	int index;
	char value;
};

class Solver
{
	public:
		static std::list<Mutation> Solve(Board& board);
		static bool UniquelySolvable(Board& board);
};

#endif