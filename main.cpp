#include "Build Settings.h"
//#include <ctime>
#include <iostream>///
#include <ctime>///
#include <ratio>///
#include <chrono>

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
	
	//SOLVE A RANDOM PUZZLE
	using namespace std;
	//cout << fixed << setprecision(3);
	
	
	//for(int i = 0; i < atoi(argv[2]); ++i) {
	//	rand();
	//}
	
	srand(time(NULL));
	cout << "starting: " << endl;
	
	Board b;
	Mutation m[BOARDSIZE];
	PuzzleGenerator::GenerateMinimum(b, m);
	
	//DummyPNet* p = new DummyPNet(1);
	std::ifstream ps("2x2/PolicyNetwork.txt");
	NeuralNet* p = new NeuralNet(ps);
	
	//DummyANet* a = new DummyANet(1);
	std::ifstream as("2x2/AssignmentNetwork.txt");
	NeuralNet* a = new NeuralNet(as);
	
	//cout << BOARDSIZE - b.Count() << endl;
	b.printBoard();
	
	Destructifier d(p, a);
	
	//clock_t begin = clock();
	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	
	//bool s = d.Destructify(b, BOARDSIZE - b.Count(), atoi(argv[3]));
	//bool s = d.Destructify(b, BOARDSIZE - b.Count(), 0);
	bool s = Solver::BruteSolve(b);
	//bool s = 0;
	//clock_t end = clock();
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	
	cout << (s ? "success" : "failed") << endl;
	
	b.printBoard();
	
	double span = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin).count();
	
	cout << span << endl;
	
	
	return 0;
	
	
}
