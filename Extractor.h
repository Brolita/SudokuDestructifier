#include "Build Settings.h"

class Extractor {
public:
	void ExtractForPolicy(Mutation m, Board& b, std::ofstream* ofs);
	void ExtractForAssignment(Mutation m, Board& b, std::ofstream* ofs);
};