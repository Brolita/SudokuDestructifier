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
	/*
	NeuralNet n(2,3,2);
	float output[] = {0.3f, 0.f};
	float input[] = {.2, .3};
	n.FeedForward(input, output);
	float desired[] = {0.5f, .1f};
	for(int i = 0; i < 5; i++)
	{
		n.BackPropagate(desired);
		n.FeedForward(input, output);
	}
	
	n.Save(std::cout);
	*/
	/*
	std::ifstream nSer("output.txt");
	NeuralNet n(nSer);
	n.Save(std::cout);
	*/
	
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
				Extractor::ExtractForPolicy(muts[i], b, ofsPolicy);
				Extractor::ExtractForAssignment(muts[i], b, ofsAssignment);
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
			cout << cursor_left(PROGRESSBAR_SIZE + 2)
				 << cursor_up(1)
				 << "Iteration " << iteration+1 << ": puzzle " 
				 << i << " size " << c
				 << "     \n" << progress << flush;
		}
		iteration++; // ayy
	}

	return 0;
}
