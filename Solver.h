#include "Build Settings.h"

#ifndef SOLVER
#define SOLVER

using namespace std;

class Mutation {
	int index;
	char value;
};

class Solver
{
	public:
		static list<Mutation> Solve(Board& board);
		static bool UniquelySolvable(Board& board);
};

#endif