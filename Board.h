#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath.h>

#ifndef BOARD
#define BOARD

class Board 
{
	public:
		int size;
		int boxSize;
		
		Board(int s);
		~Board();
		
		int Index(int x, int y);
		int Index(int ox, int oy, int bx, int by);
		
		char& Get(int index);
		char& Get(int x, int y);
		char& Get(int ox, int oy, int bx, int by);
		char& operator[] (int index);
		char& operator[] (int x, int y);
		char& operator[] (int ox, int oy, int bx, int by);
		
		// extraction for Assignment Neural Net
		char*** Dependency(int index);
		char*** operator() (int index);
		
		// extraction for Policy Neural Net
		//...
	private:
		struct char* data;
}

#endif