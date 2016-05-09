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

Destructifier::Destructifier(NeuralNet* p, NeuralNet* a) {
	policy = p;
	assign = a;
}

bool Destructifier::Destructify(Board& board, int unfilled) {
	/*
	for(int i = 0; i < unfilled; ++i) {
		std::cout << ' ';
	}
	std::cout << '*' << std::endl;
	*/
	//std::cout << unfilled << std::endl;
	
	//if(unfilled == 0) return board.isValid();
	if(unfilled == 0) {
		//std::cout << unfilled << ": all spaces filled; validate " << board.isValid() << std::endl;
		return board.isValid();
	}
	
	float policyInput[BOARDSIZE]; //create policy NN input from board
	for(int bi = 0; bi < BOARDSIZE; ++bi) {
		policyInput[bi] = (bool)board[bi];
	}
	
	float policyOutput[BOARDSIZE];
	policy->FeedForward(policyInput, policyOutput); //get output from policy NN
	
	//float maxPolicy = 0; //find highest prob cell
	//int argmaxPolicy = -1;
	/*
	for(int bi = 0; bi < BOARDSIZE; ++bi) {
		if(policyOutput[bi] > maxPolicy) { 
			maxPolicy = policyOutput[bi];
			argmaxPolicy = bi;
		}
	}
	*/
	int indices[BOARDSIZE]; //map indices and sort policy NN output
	for(int ii = 0; ii < BOARDSIZE; ++ii) {
		indices[ii] = ii;
	}
	sortProbs(policyOutput, indices, BOARDSIZE);
	
	for(int pi = 0; pi < BOARDSIZE; ++pi) {
		if(policyOutput[pi] < PRUNE_POLICY) {
			//std::cout << unfilled << ": no spaces; return 0" << std::endl;
			return 0;
		}
		
		float assignInput[DEPENDENCYSIZE]; //create assignment NN input from dependencies
		int ni = 0;
		for(int gi = 0; gi < 3; ++gi) {
			for(int di = 0; di < SIZE - 1; ++di) {
				for(int bi = 0; bi < SIZE; ++bi) {
					assignInput[ni] = (bi + 1 == *(board.dependencies[indices[pi]][gi][di]));
					++ni;
				}
			}
		}
		
		float assignOutput[SIZE];
		assign->FeedForward(assignInput, assignOutput); //get output from assignment NN
		
		int vals[SIZE]; //map indices in assignOutput to cell values
		for(int vi = 0; vi < SIZE; ++vi) {
			vals[vi] = vi + 1;
		}
		sortProbs(assignOutput, vals, SIZE);
		
		for(int ai = 0; ai < SIZE; ++ai) { //assign value & recurse
			if(assignOutput[ai] < PRUNE_ASSIGN) {
				//std::cout << unfilled << ": no value; break" << std::endl;
				//board.printBoard();
				//std::cout << indices[pi] << '\t' << ai << std::endl;
				//exit(0);
				break;
			}
			
			board[indices[pi]] = vals[ai];
			if(assignOutput[ai] > VALIDATE_ASSIGN || board.isValid()) { //isValid() called if needed
				if(Destructify(board, unfilled - 1)) { //recurse
					//std::cout << unfilled << ": recursion success: return 1" << std::endl;
					return 1; //recurse
				}
			}
			
			board[indices[pi]] = 0;
		}
	}
	
	//std::cout << unfilled << ": reached end; return 0" << std::endl;
	return 0;
}

//lazy n^2 sort
void Destructifier::sortProbs(float p[], int v[], int length) {
	float temp_p;
	int temp_v;
	
	bool sorted = 0;
	
	while(sorted == 0) {
		sorted = 1;
		
		for(int i = 0; i < length - 1; ++i) {
			if(p[i] < p[i+1]) {
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

