#include "Build Settings.h"


template<class T>
void InplaceShuffle(T* a, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
		T t = a[j];
		a[j] = a[i];
		a[i] = t;
	}
}

void PuzzleGenerator::GenerateMinimum(Board& board, Mutation muts[BOARDSIZE])
{	 
	board.Clear();
	
	PuzzleGenerator::GenerateFull(board, muts);

	InplaceShuffle(muts, BOARDSIZE);
	
	Board completeBoard(board);
	PuzzleGenerator::SearchMinimum(board, completeBoard, muts);
}
	
void PuzzleGenerator::GenerateFull(Board& board, Mutation muts[BOARDSIZE])
{	
	PlaceNext(1, 0, board, muts);
}

bool PuzzleGenerator::PlaceNext
(
	  int      insert
	, int      b
	, Board&   board
	, Mutation muts[BOARDSIZE]
)
{	
	if(insert == SIZE+1) // end condition
		return true;
	
	int by = b / BOXSIZE, bx = b % BOXSIZE;	// box from iterator
	int count = 0;
	int candidates[SIZE]; // candidates positions in the box, max of the size of the box

	for(int o = 0; o < SIZE; o++)
	{
		int oy = o / BOXSIZE, ox = o % BOXSIZE;	// position in the box

		int index = board.Index(bx,by,ox,oy);	// get index in the box

		bool placeable = board[index] == 0		// if I can place the value
			&& Solver::CanPlace(insert, board, index);

		if(placeable) 
		{
			candidates[count] = index; // push the candidate
			count++;
		}
	}
	
	InplaceShuffle(candidates, count); // random inplace shuffle

	if(count == 0) // can't place any values
		return false;
	
	Mutation* m = &(muts[(insert-1)*SIZE + b]); // next mutation
	
	for(int i = 0; i < count; i++) // for each candidate position
	{
		board[candidates[i]] = insert; // place value
		m->index = candidates[i]; // save mutation
		m->value = insert; 
		
		//std::std::cout << move_to(BOARD1Y, BOARD1X); // print board
		//board.printBoard();
		
		if(PlaceNext(insert + (b + 1)/SIZE, (b + 1)%SIZE, board, muts)) // recurse
			return true;
		
		board[candidates[i]] = 0; // not successful, unset
		m->index = -1;
		m->value = 0;
	}
	
	return false;
}

void PuzzleGenerator::SearchMinimum(
	  Board&         board
	, Board&         completeBoard
	, const Mutation muts[BOARDSIZE]
)
{	
	int r;
	bool removed[BOARDSIZE] = { false };
	for(int at = 0; at < BOARDSIZE; at++)
	{
		for(r = 0; r < BOARDSIZE; r++)
		{
			board.Clear();
			for(int i = 0; i < BOARDSIZE; i++)
			{
				if(!removed[i])
					board.Apply(muts[i]);
			}
			if(!removed[r])
			{
				board.UnApply(muts[r]);
				if(!Solver::AltSolve(board, completeBoard))
				{
					removed[r] = true;
					break;
				}
			}
		}
		if(!removed[r])
		{
			board.Clear();
			for(int i = 0; i < BOARDSIZE; i++)
			{
				if(!removed[i])
					board.Apply(muts[i]);
			}
			return ;
		}
	}
}

