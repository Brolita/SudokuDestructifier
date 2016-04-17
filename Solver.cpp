/*
      _____________
     /             \
	/               \
   /       ___       \
  /   ___ / S \ ___   \
 /   / S \\___// K \   \
 \   \___/     \___/   /
  \___________________/
    \  |  \| |/  |  /
     \ |         | /
      \|         |/
I'm literally too good TM
*/

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
				nbranch += CanPlace(vi, board.dependencies[ci]);
			}
			
			if(nbranch == 0) //empty cell with no possible value
			{
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
	
	for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
	{
		if(CanPlace(vi, board.dependencies[argminbranch])) //for each allowed value
		{
			board[argminbranch] = vi;
			if(Solve(board)) return 1; //recurse with mutation added
			else board[argminbranch] = 0; //roll back mutation & try different value
		}
	}
	return 0; //empty cell with no possible value
}

bool Solver::BruteSolve(Board& board) { //DFS for testing Solve()
	using namespace std;
	bool solved = 1;
	for(int ci = 0; ci < BOARDSIZE; ++ci) {
		if(board[ci] == 0) {
			solved = 0;
			for(int vi = 1; vi < SIZE + 1; ++vi) {
				if(Solver::CanPlace(vi, board.dependencies[ci])) {
					board[ci] = vi;
					if(BruteSolve(board)) return 1;
					else board[ci] = 0;
				}
			}
		}
	}
	return solved;
}
