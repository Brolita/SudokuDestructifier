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

bool Solver::CanPlace(char value, char* dependency[3][SIZE-1]) 
{	
	for(int j = 0; j < 3; j++)
	{
		for(int k = 0; k < SIZE - 1; k++)
		{
			if((*(dependency[j][k])) == value) {
				return false;
			}
		}
	}
	return true;
}

bool Solver::Solve(Board& board) //for use by AltSolve
{
	int minbranch = NULLHI;
	int argminbranch = -1;
	bool vals[SIZE+1]; //to ensure CanPlace is called as infrequently as possible
	bool minvals[SIZE+1];
	for(int ci = 0; ci < BOARDSIZE; ++ci) //for each cell
	{
		if(board[ci] == 0) //for each empty cell
		{
			int nbranch = 0;
			for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
			{
				if(nbranch == minbranch) //already too far
				{
					break;
				}
				vals[vi] = CanPlace(vi, board.dependencies[ci]);
				nbranch += vals[vi];
			}
			
			if(nbranch == 0) //empty cell with no possible value
			{
				return 0;
			}
			if(nbranch < minbranch) //best cell so far
			{
				minbranch = nbranch;
				argminbranch = ci;
				for(int vi = 1; vi < SIZE + 1; ++vi) minvals[vi] = vals[vi];
			}
		}
	}
	if(argminbranch == -1) return 1; //no unfilled cells remain
	
	for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
	{
		if(minvals[vi]) //for each allowed value
		{
			board[argminbranch] = vi;
			if(Solve(board)) return 1; //recurse with mutation added
			else board[argminbranch] = 0; //roll back mutation & try different value
		}
	}
	return 0; //empty cell with no possible value
}

bool Solver::Solve(Board& board, Mutation sol[BOARDSIZE]) {
	Solve(board, sol, 0);
}

bool Solver::Solve(Board& board, Mutation sol[BOARDSIZE], int solSize)
{
	int minbranch = NULLHI;
	int argminbranch = -1;
	bool vals[SIZE+1]; //to ensure CanPlace is called as infrequently as possible
	bool minvals[SIZE+1];
	for(int ci = 0; ci < BOARDSIZE; ++ci) //for each cell
	{
		if(board[ci] == 0) //for each empty cell
		{
			int nbranch = 0;
			for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
			{
				if(nbranch == minbranch) //already too far
				{
					break;
				}
				vals[vi] = CanPlace(vi, board.dependencies[ci]);
				nbranch += vals[vi];
			}
			
			if(nbranch == 0) //empty cell with no possible value
			{
				return 0;
			}
			if(nbranch < minbranch) //best cell so far
			{
				minbranch = nbranch;
				argminbranch = ci;
				for(int vi = 1; vi < SIZE + 1; ++vi) minvals[vi] = vals[vi];
			}
		}
	}
	if(argminbranch == -1) return 1; //no unfilled cells remain
	
	for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
	{
		if(minvals[vi]) //for each allowed value
		{
			board[argminbranch] = vi;
			if(Solve(board, sol, solSize + 1)) { //recurse with mutation added
				sol[solSize].index = argminbranch; //record mutation
				sol[solSize].value = vi;
				return 1;
			}
			else board[argminbranch] = 0; //roll back mutation & try different value
		}
	}
	return 0; //empty cell with no possible value
}


bool Solver::AltSolve(Board& board, Board& completeBoard)
{
	int minbranch = NULLHI;
	int argminbranch = -1;
	bool vals[SIZE+1]; //to ensure CanPlace is called as infrequently as possible
	bool minvals[SIZE+1];
	for(int ci = 0; ci < BOARDSIZE; ++ci) //for each cell
	{
		if(board[ci] == 0) //for each empty cell
		{
			int nbranch = 0;
			for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
			{
				if(nbranch == minbranch) //already too far
				{
					break;
				}
				//count values that can be placed but aren't in completeBoard:
				vals[vi] = CanPlace(vi, board.dependencies[ci]) && completeBoard[ci] != vi;
				nbranch += vals[vi];
			}
			
			if(nbranch < minbranch) //best cell so far
			{
				minbranch = nbranch;
				argminbranch = ci;
				for(int vi = 1; vi < SIZE + 1; ++vi) minvals[vi] = vals[vi];
			}
			if(minbranch == 0) //found a guaranteed value; apply it
			{
				break;
			}
		}
	}
	//if(argminbranch == -1 && board != completeBoard) return 1; //no unfilled cells remain
	//else if(argminbranch == -1) return 0;
	if(argminbranch == -1) { //no unfilled cells remain
		if(board != completeBoard) {
			std::cout << clear_screen;
			
			std::cout << move_to(BOARD2Y, BOARD1X);
			board.printBoard();
			
			std::cout << move_to(BOARD2Y, BOARD2X);
			completeBoard.printBoard();
			
			exit(0);
			
		}
		return board != completeBoard; //return whether alt solution was found
	}
	
	
	for(int vi = 1; vi < SIZE + 1; ++vi) //for each value
	{
		if(minvals[vi]) //for each allowed value
		{
			board[argminbranch] = vi;
			if(Solve(board)) return 1; //recurse with mutation added
			else board[argminbranch] = 0; //roll back mutation & try different value
		}
	}
	//
	board[argminbranch] = completeBoard[argminbranch]; //this mutation is guaranteed
	//if(AltSolve(board, completeBoard)) return 1; //recurse with mutation added
	//return 0; //empty cell with no possible value
	return AltSolve(board, completeBoard); //recurse with guaranteed mutation added
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
