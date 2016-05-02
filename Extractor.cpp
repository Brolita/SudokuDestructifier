#include <iostream>

void Extractor::ExtractForAssignment(Mutation m, Board& b) {

	int len = 3*(SIZE-1)*SIZE + SIZE;

	char output[len];

	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < SIZE-1; k++) {
			bool berOptions[SIZE];
			SetBool(berOptions, *(b.depencies[m.value][j][k]));
			for (int i = 0; i < SIZE; i++) {
				output[(j*(SIZE-1) + k)*SIZE + i] = berOptions[i] + '0';
			}
		}
	}

	std::cout << "[";
	for (int i = 0; i < len; i++) {
		std::cout << " " << output[i];
	}
	std::cout << " ]\n";
}