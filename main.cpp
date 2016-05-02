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
	exit(1);
}

void readFile(std::ifstream &file) {
	Board b;
	char a;
	int i = 0;
	int count = 0;
	Mutation m;
	while (file) {
		file.get(a);
		if (i == BOARDSIZE) {
			count++;
			if (count%10000 == 0)
				b.printBoard();
			i = 0;
			b.Clear();
		}
		m.index = i;
		m.value = a;
		b.Apply(m);
		i++;
	}
	//b.printBoard();
	file.close();
}


int main(int argc, char* argv[])
{
	using namespace std;
	signal (SIGINT,onCtrlC);
	
	srand (time(NULL));
	
	int c = 0;
	
	ofs = new ofstream("output3x3.bin", std::ofstream::out | std::ofstream::app);
	
	string progress = string("[") 
		+ string(PROGRESSBAR_SIZE - 1, ' ') + string("]");
	
	Board b;
	Mutation muts[BOARDSIZE];
	//std::ifstream file;
	//file.open("output3x3o1.bin",std::ios::binary);
	//readFile(file);
	/*
	while(1)
	{
		for(int i = 0; i < PER_TICK * PROGRESSBAR_SIZE; i++)
		{
			PuzzleGenerator::GenerateMinimum(b, muts);
			ofs->write(b.data, BOARDSIZE);
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
				 << "Iteration " << c+1 << ": " << i
				 << "     \n" << progress << flush;
		}
		c++; // ayy
	}
	*/

	return 0;
}
