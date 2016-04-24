#include "Build Settings.h"

#ifndef PUZZLE_GENERATOR
#define PUZZLE_GENERATOR

class PuzzleGenerator
{
	public:
		static void GenerateMinimum(Board& board, Mutation muts[BOARDSIZE]);
	private:
		static void GenerateFull(Board& board, Mutation muts[BOARDSIZE]);
		static bool PlaceNext(int insert, int b, Board& board, Mutation muts[BOARDSIZE]);
		static int BinarySearchMinimum(Board& board, Board& completeBoard,
				const Mutation muts[BOARDSIZE], int at, int remaining);

};

#endif