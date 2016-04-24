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
		
		int Index(int x, int y);
		int Index(int ox, int oy, int bx, int by);
		
		char& Get(int index);
		char& Get(int x, int y);
		char& Get(int ox, int oy, int bx, int by);
		char& operator[] (int index);
		
		bool isValid();
		void printBoard();
		
		void Clear();
		void Apply(Mutation m);
		
		// extraction for Assignment Neural Net 
		// also useful for solution algorithms
		char* dependencies[BOARDSIZE][3][SIZE-1];
		
		// extraction for Policy Neural Net
		void SolvedPositions(bool o[BOARDSIZE]);
	private:
		char data[BOARDSIZE];
		void Dependency(int index, char* v[3][SIZE - 1]);
};

bool operator==(Board& b1, Board& b2);
bool operator!=(Board& b1, Board& b2);

#endif
