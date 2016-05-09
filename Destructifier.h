#include "Build Settings.h"

#ifndef DESTRUCTIFIER
#define DESTRUCTIFIER

class Destructifier
{
	public:
		Destructifier(NeuralNet* p, NeuralNet* a);
		bool Destructify(Board& board, int unfilled);
	
	private:
		NeuralNet* policy;
		NeuralNet* assign;
		
		void sortProbs(float f[], int i[], int length); 
};

#endif