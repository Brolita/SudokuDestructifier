#include "Build Settings.h"

void onCtrlC(int i)
{
	std::cout << reset << "\n\n  Kill signal detected\n" << std::endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	using namespace std;
	srand (time(NULL));
	
	clock_t start = clock(), diff;
	
	for(int i = 0; i < 100; i++) 
		Board b = PuzzleGenerator::GenerateMinimum();
	
	diff = clock() - start;

	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);
	
	return 0;
	
	signal (SIGINT,onCtrlC);
	
	std::cout << clear_screen
			  << move_to(1,1)
			  << "I'm using vt100.h for this magic, fyi\n"
			  << "Generating a full board" << std::endl;
	
	srand (time(NULL));
	while(1)
	{
		Board b = PuzzleGenerator::GenerateMinimum();
		std::cout << move_to(BOARD2Y, 1);
		b.printBoard();
	}
}
