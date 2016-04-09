#include "Build Settings.h"

#ifndef BOARD
#define BOARD

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
		
		// extraction for Assignment Neural Net
		char*** Dependency(int index);
		char*** operator() (int index);
		
		// extraction for Policy Neural Net
		//...
	private:
		char* data;
};

#endif