#include "Build Settings.h"

#ifndef DUMMYANET
#define DUMMYANET

class DummyANet 
{
	public:
		DummyANet(bool a);
		
		void FeedForward(float* input, float* output);
	private:
		bool inst;
};

#endif