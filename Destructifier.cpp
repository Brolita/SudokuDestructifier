/*
     __________
    /    S     \
   /    /       \
  /     \__      \
 /      /  \___   \
 \      \__/   \  /
  \S____/       K/
   \            /
    \__________/
I'm literally too good TM
*/

#include "Build Settings.h"

//Destructifier::Destructifier(DummyPNet* p, DummyANet* a) {
//Destructifier::Destructifier(DummyPNet* p, NeuralNet* a) {
Destructifier::Destructifier(NeuralNet* p, DummyANet* a) {
	policy = p;
	assign = a;
}

bool Destructifier::Destructify(Board& board, int unfilled, bool debug) {
	if(unfilled == 0) { //all spaces filled
		if(debug) std::cout << unfilled << ": all spaces filled; validate = " << board.isValid() << std::endl;
		return board.isValid();
	}
	
	double policyInput[BOARDSIZE]; //create policy NN input from board
	for(int bi = 0; bi < BOARDSIZE; ++bi) {
		policyInput[bi] = (bool)board[bi];
	}
	
	double policyOutput[BOARDSIZE];
	policy->FeedForward(policyInput, policyOutput); //get output from policy NN
	
	int indices[BOARDSIZE]; //map indices and sort policy NN output
	for(int ii = 0; ii < BOARDSIZE; ++ii) {
		indices[ii] = ii;
	}
	sortProbs(policyOutput, indices, BOARDSIZE);
	
	/*
	for(int i = 0; i < BOARDSIZE; ++i) {
		std::cout << policyOutput[i] << '\t'
	}
	std::cout << std::endl;
	*/
	for(int i = 0; i < BOARDSIZE; ++i) {
		std::cout << indices[i] << '\t';
	}
	//return 0;
	std::cout << std::endl << std::endl;
	
	
	for(int pi = 0; pi < BOARDSIZE; ++pi) {
		int num = 0;
		for(int gi = 0; gi < 3; ++gi) {
			for(int di = 0; di < SIZE - 1; ++di) {
				num += *(board.dependencies[indices[pi]][gi][di]) > 0 ? 1 : 0;
//				std::cout << (int)*(board.dependencies[indices[pi]][gi][di]) << '\t';
//				std::cout << std::endl;
			}
//		std::cout << std::endl;
		}
//		std::cout << std::endl;
		std::cout << num << '\t';
	}
	std::cout << std::endl;
	return 0;
	
	for(int pi = 0; pi < BOARDSIZE; ++pi) {
		if(board[indices[pi]] == 0) { //only operate on empty spaces
			if(policyOutput[pi] < PRUNE_POLICY) { //no spaces to fill
				if(debug) std::cout << unfilled << ": no spaces to fill; return 0" << std::endl;
				return 0;
			}
			
			double assignInput[DEPENDENCYSIZE]; //create assignment NN input from dependencies
			int ni = 0;
			for(int gi = 0; gi < 3; ++gi) {
				for(int di = 0; di < SIZE - 1; ++di) {
					//std::cout << (int)*(board.dependencies[indices[pi]][gi][di]) << ":\t";
					for(int bi = 0; bi < SIZE; ++bi) {
						assignInput[ni] = (bi + 1 == *(board.dependencies[indices[pi]][gi][di]));
						//std::cout << assignInput[ni] << ' ';
						++ni;
					}
					//std::cout << std::endl;
				}
			}
			//std::cout << std::endl;
			
			double assignOutput[SIZE];
			assign->FeedForward(assignInput, assignOutput); //get output from assignment NN
			
			/*
			std::cout << indices[pi] << ":\t";
			for(int i = 0; i < SIZE; ++i) {
				std::cout << assignOutput[i] << '\t';
			}
			std::cout << std::endl;
			return 0;
			*/
			
			int vals[SIZE]; //map indices in assignOutput to cell values
			for(int vi = 0; vi < SIZE; ++vi) {
				vals[vi] = vi + 1;
			}
			sortProbs(assignOutput, vals, SIZE);
			
			for(int ai = 0; ai < SIZE; ++ai) { //assign value & recurse
				if(assignOutput[ai] < PRUNE_ASSIGN) { //no value to assign
					if(debug) std::cout << unfilled << ": no value to assign; break" << std::endl;
					break;
				}
				
				board[indices[pi]] = vals[ai];
				if(assignOutput[ai] > VALIDATE_ASSIGN || board.isValid()) { //isValid() called if needed
					if(debug) std::cout << unfilled << ": value assigned; recurse" << std::endl;
					if(Destructify(board, unfilled - 1, debug)) { //recurse
						if(debug) std::cout << unfilled << ": recursion sucess; return 1" << std::endl;
						return 1; //recurse
					}
				}
				
				board[indices[pi]] = 0;
			}
		}
	}
	
	if(debug) std::cout << unfilled << ": reached end (filled space?); return 0" << std::endl;
	return 0; //reached end
}

//lazy n^2 sort
void Destructifier::sortProbs(double p[], int v[], int length) {
	double temp_p;
	int temp_v;
	
	bool sorted = 0;
	
	while(sorted == 0) {
		sorted = 1;
		
		for(int i = 0; i < length - 1; ++i) {
			if(p[i] < p[i+1] || (std::isnan(p[i]) && !std::isnan(p[i+1]))) {
				sorted = 0;
				
				temp_p = p[i];
				temp_v = v[i];
				
				p[i] = p[i+1];
				v[i] = v[i+1];
				
				p[i+1] = temp_p;
				v[i+1] = temp_v;
			}
		}
	}
}

