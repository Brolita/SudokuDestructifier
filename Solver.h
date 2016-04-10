#include "Build Settings.h"

#ifndef SOLVER
#define SOLVER

class Solver
{
	public:
		static void Solve(Board& board, Mutation muts[boardSize]);
		static bool CanPlace(char value, char* dependency[3][size-1]);
};

#endif