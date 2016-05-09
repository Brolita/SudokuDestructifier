#include "Build Settings.h"

#ifndef DUMMYANET
#define DUMMYANET

class DummyANet 
{
	public:
		DummyANet(bool a);
		
		void FeedForward(double* input, double* output);
	private:
		bool inst;
};

#endif