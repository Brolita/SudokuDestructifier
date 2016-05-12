#include "Build Settings.h"

#define PROGRESSBAR_SIZE 100
#define PER_TICK 10

std::ofstream* ofsPolicy;
std::ofstream* ofsAssignment;

void SetBool(bool a[SIZE], char v) {
	
    for(int i = 0; i < SIZE; i++)
        a[i] = false;
    if(v)
        a[v-1] = true;
}

void onCtrlC(int i)
{
	ofsPolicy->close();
	ofsAssignment->close();
	exit(1);
}

void ReadBoard(std::ifstream &file, Board& b) {
	char a;
	int i = 0;
	while (file) {
		file.get(a);
		if (i == BOARDSIZE)
			return;
		b[i] = a;
		i++;
	}
}


int main(int argc, char* argv[])
{
	//GENERATE DATA
	using namespace std;
	signal (SIGINT,onCtrlC);
	
	srand (time(NULL));
	
	int iteration = 0;
	
	ofsPolicy = new ofstream(argv[1], std::ofstream::out | std::ofstream::app);
	ofsAssignment = new ofstream(argv[2], std::ofstream::out | std::ofstream::app);
	
	string progress = string("[") 
		+ string(PROGRESSBAR_SIZE - 1, ' ') + string("]");
	
	Board b;
	Mutation muts[BOARDSIZE];
	cout << clear_screen;
	while(1)
	{
		for(int i = 0; i < PER_TICK * PROGRESSBAR_SIZE; i++)
		{
			PuzzleGenerator::GenerateMinimum(b, muts);
			int c = 0;
			for(int j = 0; j < BOARDSIZE; j++)
			{
				if(b[j]) c++;
			}
			Solver::Solve(b, muts);
			for(int j = 0; j < c; j++)
			{
				// undo solution
				b[muts[j].index] = 0;
			}
			for(int j = 0; j < c; j++)
			{
				Extractor::ExtractForPolicy(muts[j], b, ofsPolicy);
				Extractor::ExtractForAssignment(muts[j], b, ofsAssignment);
				b.Apply(muts[j]);
			}
			
			if(i % PER_TICK == 0)
			{
				if(i == 0)
				{
					progress = std::string("[") 
						+ std::string(PROGRESSBAR_SIZE, ' ') 
						+ std::string("]");
				}
				else
				{
					progress[i/PER_TICK] = '#';
				}
			}
			cout << move_to(2,1)
				 << "Iteration " << iteration+1 << ": puzzle " 
				 << i << " size " << c
				 << "     \n" << progress << flush;
			
			if(iteration * PROGRESSBAR_SIZE * PER_TICK + i > 1000000)
			{
				return 0;
			}
		}
		iteration++; // ayy
	}
	
	return 0;
}
