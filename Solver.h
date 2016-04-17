#include "Build Settings.h"

#ifndef SOLVER
#define SOLVER

class Solver
{
	public:
		static void Solve(Board& board);
		static void AltSolve(Board& board, Mutation muts[boardSize]);
		static int SolutionCount(Board& board);
		static bool CanPlace(char value, char* dependency[3][size-1]);
};

#endif