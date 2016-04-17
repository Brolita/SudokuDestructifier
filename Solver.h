#include "Build Settings.h"

#ifndef SOLVER
#define SOLVER

class Solver
{
	public:
		static bool Solve(Board& board);
		static bool BruteSolve(Board& board);
		static bool AltSolve(Board& board, Mutation muts[boardSize]);
		static int SolutionCount(Board& board);
		static bool CanPlace(char value, char* dependency[3][size-1]);
};

#endif