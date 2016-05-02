#include "Board.h"

#include <string>

class Extractor {
public:
	void ExtractForPolicy();
	void ExtractForAssignment(Mutation m, Board& b);
};