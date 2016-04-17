#include "Build Settings.h"

void GenerateFull(Board& board, Mutation muts[boardSize]);

template<class T>
void InplaceShuffle(T* a, size_t n);

bool PlaceNext(int insert, int b, Board& board, Mutation muts[boardSize]);

void BinarySearchMinimum(Board& board, Mutation muts[boardSize],
	int at, int remaining);

Board PuzzleGenerator::GenerateMinimum()
{
	Board board;
	Mutation muts[boardSize];
	
	GenerateFull(board, muts);
	
	InplaceShuffle(muts, boardSize);
	
	//generate half-full puzzle:
	//BinarySearchMinimum(board, muts, BOARDSIZE/2, boardSize);
	BinarySearchMinimum(board, muts, 20, boardSize); //probably few solutions for size 3
	
	return board;
}
	
void GenerateFull(Board& board, Mutation muts[boardSize])
{	
	PlaceNext(1, 0, board, muts);
}

bool PlaceNext(int insert, int b, Board& board, Mutation muts[boardSize])
{	
	if(insert == size+1)
		return true;
	
	int by = b / boxSize, bx = b % boxSize;
	int count = 0;
	int candidates[size];
	for(int o = 0; o < size; o++)
	{
		int oy = o / boxSize, ox = o % boxSize;
		int index = board.Index(bx,by,ox,oy);
		bool placeable = board[index] == 0
			&& Solver::CanPlace(insert, board.dependencies[index]);
		if(placeable) 
		{
			candidates[count] = index;
			count++;
		}
	}
	
	InplaceShuffle(candidates, count);
	
	if(count == 0)
		return false;
	
	Mutation* m = &(muts[(insert-1)*size + b]);
	
	for(int i = 0; i < count; i++) 
	{
		board[candidates[i]] = insert;
		m->index = candidates[i];
		m->value = insert;
		
		std::cout << move_to(board1y, 1);
		board.printBoard();
		
		if(PlaceNext(insert + (b + 1)/size, (b + 1)%size, board, muts))
			return true;
		
		board[candidates[i]] = 0;
		m->index = -1;
		m->value = 0;
	}
	
	return false;
}

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

void BinarySearchMinimum(Board& board, Mutation muts[boardSize],
	int at, int remaining)
{
	board.Clear();
	for(int i = 0; i < at; i++)
	{
		board.Apply(muts[i]);
	}
}

