#include "Build Settings.h"

#ifndef DESTRUCTIFIER
#define DESTRUCTIFIER

class Destructifier
{
	public:
		Destructifier(DummyPNet* p, DummyANet* a);
		bool Destructify(Board& board, int unfilled);
	
	private:
		DummyPNet* policy;
		DummyANet* assign;
		
		void sortProbs(float f[], int i[], int length); 
};

#endif