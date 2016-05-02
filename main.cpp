#include "Build Settings.h"

#define PROGRESSBAR_SIZE 100
#define PER_TICK 10

std::ofstream* ofs;

void SetBool(bool a[SIZE], char v) {
    for(int i = 0; i < SIZE; i++)
        a[i] = false;
    if(v)
        a[v-1] = true;
}

void onCtrlC(int i)
{
	ofs->close();
	delete ofs;
	exit(1);
}

int main(int argc, char* argv[])
{
	signal (SIGINT,onCtrlC);
	
	srand (time(NULL));
	
	ofs = new ofstream(argv[1], std::ofstream::out | std::ofstream::app);
	
	Board b;
	Mutation muts[BOARDSIZE];
	while(1)
	{
		for(int i = 0; i < PER_TICK * PROGRESSBAR_SIZE; i++)
		{
			PuzzleGenerator::GenerateMinimum(b, muts);
			ofs->write(b.data, BOARDSIZE);
		}
	}

	return 0;
}
