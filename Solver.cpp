#include <typeinfo>
#include "Build Settings.h"

bool Solver::CanPlace(char value, char* dependency[3][size-1]) 
{	
	for(int j = 0; j < 3; j++)
	{
		for(int k = 0; k < size - 1; k++)
		{
			if((*(dependency[j][k])) == value) {
				return false;
			}
		}
	}
	return true;
}

bool Solver::Solve(Board& board)
{
	int minbranch = NULLHI;
	int argminbranch = -1;
	for(int ci = 0; ci < BOARDSIZE; ++ci) //for each cell
	{
		if(board[ci] == 0) //for each empty cell
		{
			int nbranch = 0;
			for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
			{
				nbranch += Solver::CanPlace(vi, board.dependencies[ci]);
			}
			
			if(nbranch == 0) //empty cell with no possible value
			{
				std::cout << "Solver::Solve: backtrack attempted" << std::endl;
				return 0;
			}
			if(nbranch < minbranch) //best cell so far
			{
				minbranch = nbranch;
				argminbranch = ci;
			}
		}
	}
	if(argminbranch == -1)  return 1; //no unfilled cells remain
	
	if(minbranch != 1)
	{
		std::cerr << "Solver::Solve WARNING: backtracking not yet verified: " << minbranch << std::endl;
	}
	
	for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
	{
		if(Solver::CanPlace(vi, board.dependencies[argminbranch])) //for each allowed value
		{
			board[argminbranch] = vi;
			if(Solver::Solve(board)) return 1; //recurse with mutation added
			else board[argminbranch] = 0; //roll back mutation; try more values before returning
		}
	}
	
	std::cerr << "Solver::solve ERORR: all values invalid for valid cell " << argminbranch << std::endl;
	//error handling?
}

bool BruteSolve(Board& board) { //for testing Solve()
	//things
}
