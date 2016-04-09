#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>

#include "Board.h"

int main(int argc, char* argv[])
{
	Board b = Board(81);
	b.printBoard();
}
