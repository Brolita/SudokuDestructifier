#include "Build Settings.h"

int main(int argc, char* argv[])
{
	srand (time(NULL));
	Board b = PuzzleGenerator::GenerateMinimum();
	b.printBoard();
}
