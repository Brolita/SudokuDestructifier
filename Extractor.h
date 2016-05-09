#include "Build Settings.h"

class Extractor {
public:
	static int Extract(Board& b, Mutation muts[BOARDSIZE], double* inputPolicy, double* outputPolicy, double* inputAssignment, double* outputAssignment);
	static void ExtractForPolicy(Mutation m, Board& b, std::ofstream* ofs);
	static void ExtractForAssignment(Mutation m, Board& b, std::ofstream* ofs);
	static void ExtractForPolicy(Mutation m, Board& b, double* input, double* output);
	static void ExtractForAssignment(Mutation m, Board& b, double* input, double* output);
};