#include "Build Settings.h"

#ifndef DUMMYPNET
#define DUMMYPNET

class DummyPNet 
{
	public:
		DummyPNet(bool p);
		
		void FeedForward(float* input, float* output);
	private:
		bool inst;
};

#endif