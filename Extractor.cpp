#include "Build Settings.h"

void Extractor::ExtractForAssignment(Mutation m, Board& b, std::ofstream* ofs) 
{
	char output[ASSIGNMENT_NN_LENGTH];

	for (int j = 0; j < 3; j++) 
	{
		for (int k = 0; k < SIZE-1; k++) 
		{
			bool berOptions[SIZE];
			SetBool(berOptions, *(b.dependencies[m.value][j][k]));
			for (int i = 0; i < SIZE; i++) 
			{
				output[(j*(SIZE-1) + k)*SIZE + i] = berOptions[i] + '0';
			}
		}
	}

	ofs->write(output, ASSIGNMENT_NN_LENGTH);
	
	/*
	std::cout << "[";
	for (int i = 0; i < len; i++) {
		std::cout << " " << output[i];
	}
	std::cout << " ]\n";*/
}