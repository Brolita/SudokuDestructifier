#include "Build Settings.h"

class Extractor {
public:
	static void ExtractForPolicy(Mutation m, Board& b, std::ofstream* ofs);
	static void ExtractForAssignment(Mutation m, Board& b, std::ofstream* ofs);
};