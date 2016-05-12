#include "Build Settings.h"
#include <chrono>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <tgmath.h>

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
	
	using namespace std;
	
	system("rm data.txt");
	system("rm debug.txt");
	
	std::fstream data;
	data.open("data.txt", std::fstream::app);
	std::fstream debug;
	debug.open("debug.txt", std::fstream::app);
	
	//for(int i = 0; i < atoi(argv[2]); ++i) {
	//	rand();
	//}
	
	//srand(time(NULL));
	srand(420);
	cout << "starting: " << endl;
	
	Board b[NPUZZLES];
	Mutation m[BOARDSIZE];
	
	for(int i = 0; i < NPUZZLES; ++i) {
		PuzzleGenerator::GenerateMinimum(b[i], m);
	}
	
	int nPolicy = 0;
	int nAssign = 0;
	
	while(nPolicy < NPOLICY) {
		
		while(nAssign < NASSIGN) {
	
			char Pname[STRBUF] = "2x2/PolicyNet";
			char Aname[STRBUF] = "2x2/AssignmentNet";
			char temp[STRBUF];
			
			//DummyPNet* p = new DummyPNet(1);
			sprintf(temp, "%d", nPolicy * TOTALPOLICY / NPOLICY);
			strcat(Pname, temp);
			strcat(Pname, ".txt");
			///std::ifstream ps(Pname);
			std::ifstream ps("2x2/PolicyNet0.txt");
			NeuralNet* p = new NeuralNet(ps);
			
			//DummyANet* a = new DummyANet(1);
			sprintf(temp, "%d", nAssign * TOTALASSIGN / NASSIGN);
			cout << temp << endl;
			strcat(Aname, temp);
			strcat(Aname, ".txt");
			///std::ifstream as(Aname);
			std::ifstream as("2x2/AssignmentNet0.txt");
			NeuralNet* a = new NeuralNet(as);
			
			Destructifier d(p, a);
			
			//Board b;
			//Mutation m[BOARDSIZE];
			
			//clock_t begin = clock();
			
			int fds[2];
			pipe(fds);
			double elapsed[NPUZZLES];
			double average = 0;
			
			for(int np = 0; np < NPUZZLES; ++np) {
				
				//PuzzleGenerator::GenerateMinimum(b, m);
				b[np].printBoard();
				
				int pid = fork();
				
				if(pid == 0) {
					timespec start, end;
					//std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
					clock_gettime(CLOCK_MONOTONIC, &start);
					
					//bool s = d.Destructify(b, BOARDSIZE - b.Count(), 0);
					bool s = d.Destructify(b[np], BOARDSIZE - b[np].Count(), 0);
					//bool s = Solver::BruteSolve(b);
					
					//std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
					clock_gettime(CLOCK_MONOTONIC, &end);
					
					cout << (s ? "success" : "failed") << endl;
					
					b[np].printBoard();
					
					//double span = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin).count();
					double span = (end.tv_sec - start.tv_sec) + pow(10, -9) * (end.tv_nsec - start.tv_nsec);
					
					cout << span << endl;
					
					write(fds[1], (void*)(&span), sizeof(double));
					
					return 0;
				}
				else {
					fd_set listen;
					timeval t = {MAXTIME, 0};
					
					FD_ZERO(&listen);
					FD_SET(fds[0], &listen);
					select(FD_SETSIZE, &listen, NULL, NULL, &t);
					
					double span;
					if(FD_ISSET(fds[0], &listen)) {
						read(fds[0], (void*)(&span), sizeof(double));
					}
					else {
						kill(pid, SIGKILL);
						span = MAXTIME;
						b[np].printBoard();
					}
					
					elapsed[np] = span;
					average += span;
					data << span << ' ' << flush;
					
					cout << "policy: " << nPolicy + 1 << " / " << NPOLICY << " (" << Pname << ")" << endl;
					cout << "assign: " << nAssign + 1 << " / " << NASSIGN << " (" << Aname << ")" << endl;
					cout << "puzzle: " << np + 1 << " / " << NPUZZLES << endl;
					cout << "time: " << span << endl;
					cout << "avg: " << average / (np + 1) << endl << endl;
				}
			}
			
			average /= NPUZZLES;
			cout << endl << "cell completed; avg = " << average << endl << endl << endl;
			
			data << endl << average << endl;
			debug << average << ' ' << flush;
			
			++nAssign;
		}
		nAssign = 0;
		++nPolicy;
		debug << endl;
	}
	
	return 0;
}
