#include "Build Settings.h"

#ifndef DESTRUCTIFIER
#define DESTRUCTIFIER

class Destructifier
{
	public:
		//Destructifier(DummyPNet* p, DummyANet* a);
		//Destructifier(DummyPNet* p, NeuralNet* a);
		Destructifier(NeuralNet* p, DummyANet* a);
		bool Destructify(Board& board, int unfilled, bool debug);
	
	private:
		//DummyPNet* policy;
		NeuralNet* policy;
		DummyANet* assign;
		//NeuralNet* assign;
		
		void sortProbs(double f[], int i[], int length); 
};

#endif