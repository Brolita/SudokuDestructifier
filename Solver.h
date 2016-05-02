#include "Build Settings.h"

#ifndef SOLVER
#define SOLVER

class Solver
{
	public:
		static bool Solve(Board& board, Mutation sol[BOARDSIZE]);
		static bool BruteSolve(Board& board);
		static bool AltSolve(Board& board, Board& completeBoard);
		static int SolutionCount(Board& board);
		static bool CanPlace(char value, char* dependency[3][SIZE-1]);
	
	private:
		static bool Solve(Board& board);
		static bool Solve(Board& board, Mutation sol[BOARDSIZE], int solSize);
};

#endif