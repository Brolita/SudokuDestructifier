#include "Build Settings.h"

void onCtrlC(int i)
{
	std::cout << reset << "\n\n  Kill signal detected\n" << std::endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	/*
	using namespace std;
	srand (time(NULL));
	Board b = PuzzleGenerator::GenerateMinimum();
	cout << clear_screen << move_to(BOARD1Y, 1);
	b.printBoard();
	bool x = Solver::Solve(b);
	cout << "final: solved? = " << x << endl;
	b.printBoard();
	return 0;
	*/
	
	signal (SIGINT,onCtrlC);
	
	std::cout << clear_screen
			  << move_to(1,1)
			  << "I'm using vt100.h for this magic, fyi\n"
			  << "Generating a full board" << std::endl;
	
	srand (time(NULL));
	while(1)
	{
		Board b = PuzzleGenerator::GenerateMinimum();
		std::cout << move_to(board2y, 1);
		b.printBoard();
	}
}
