#include "Build Settings.h"
#define print(i, j, v) std::cout << __LINE__ << ": " << j << ", " << i << " = " << v << std::endl

DummyPNet::DummyPNet(bool p)
{
	inst = p;
}

void DummyPNet::FeedForward(float* input, float* output) 
{
	//lol i don't even care get at me
	if(!inst) return;
	int maxDep = 0;
	
	for(int ci = 0; ci < BOARDSIZE; ++ci) {
		int row = ci / SIZE;
		int col = ci % SIZE;
		int rbox = row / BOXSIZE;
		int cbox = col / BOXSIZE;
		int box = rbox * BOXSIZE + cbox;
		int numDep = 0;
		
		for(int di = 0; di < SIZE; ++di) {
			numDep += (bool)input[row * SIZE + di];
			numDep += (bool)input[di * SIZE + col];
		}
		
		for(int bi = 0; bi < BOARDSIZE; ++bi) {
			int b_row = bi / SIZE;
			int b_col = bi % SIZE;
			int b_rbox = b_row / BOXSIZE;
			int b_cbox = b_col / BOXSIZE;
			int b_box = b_rbox * BOXSIZE + b_cbox;
			if(b_box == box && bi != ci) {
				numDep += (bool)input[bi];
			}
		}
		
		if(input[ci]) {
			numDep = 0;
		}
		
		if(numDep > maxDep) {
			maxDep = numDep;
		}
		
		output[ci] = numDep;
	}
	
	for(int ci = 0; ci < BOARDSIZE; ++ci) {
		output[ci] /= maxDep;
	}
}

