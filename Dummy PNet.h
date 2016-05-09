#include "Build Settings.h"

#ifndef DUMMYPNET
#define DUMMYPNET

class DummyPNet 
{
	public:
		DummyPNet(bool p);
		
		void FeedForward(double* input, double* output);
	private:
		bool inst;
};

#endif