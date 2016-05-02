#include "Build Settings.h"

class Extractor {
public:
	void ExtractForPolicy();
	void ExtractForAssignment(Mutation m, Board& b, std::ofstream* ofs);
};