#include "Build Settings.h"

#ifndef BOARD
#define BOARD

class Mutation {
	public:
		int index;
		char value;
};

class Board 
{
	public:
		Board();
		Board(const Board& b);
		
		int Index(int x, int y);
		int Index(int ox, int oy, int bx, int by);
		
		char& Get(int index);
		char& Get(int x, int y);
		char& Get(int ox, int oy, int bx, int by);
		char& operator[] (int index);

		Board& operator=(const Board& b);
		
		int Count();
		
		bool isValid();
		void printBoard();
		
		void Clear();
		void Apply(Mutation m);
		void UnApply(Mutation m);
		
		// extraction for Assignment Neural Net 
		// also useful for solution algorithms
		char GetDependency(int i, int j, int k);
		char* dependencies[BOARDSIZE][3][SIZE-1];
		
		char data[BOARDSIZE];
	private:
		void Dependency(int index);
};

bool operator==(Board& b1, Board& b2);
bool operator!=(Board& b1, Board& b2);

#endif
