#include "Build Settings.h"

class Extractor {
public:
	static int GetExtractions(Board& b, Mutation muts[BOARDSIZE], float* inputPols, float* outputPols, float* inputAsns, float* outputAsns);
	static void ExtractForPolicy(Mutation m, Board& b, float* input, float* output);
	static void ExtractForAssignment(Mutation m, Board& b, float* input, float* output);
};