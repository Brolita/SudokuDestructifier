#include "Build Settings.h"
#define print(i, j, v) std::cout << __LINE__ << ": " << j << ", " << i << " = " << v << std::endl

DummyANet::DummyANet(bool a)
{
	inst = a;
}

void DummyANet::FeedForward(float* input, float* output) 
{
	//fight me nerd
	if(!inst) return;
	
	for(int oi = 0; oi < SIZE; ++oi) {
		output[oi] = 1;
	}
	
	for(int ii = 0; ii < 3 * (SIZE - 1) * SIZE; ++ii) {
		if(input[ii]) {
			output[ii % SIZE] = 0;
		}
	}
}

