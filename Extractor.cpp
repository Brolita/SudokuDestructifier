#include "Build Settings.h"

void Extractor::ExtractForPolicy(Mutation m, Board& b, std::ofstream* ofs) {
	char output[BOARDSIZE];
	char outputM[BOARDSIZE];
	
	for (int i = 0; i < BOARDSIZE; i++) 
	{
		
		if (b.Get(i) == 0) 
			output[i] = '0';
		else 
			output[i] = '1';
		
		if (m.index == i)
			outputM[i] = '1';
		else 
			outputM[i] = '0';
	}
}

void Extractor::ExtractForPolicy
(
	  Mutation m
	, Board&   b
	, float*   input
	, float*   output
) 
{
	for (int i = 0; i < BOARDSIZE; i++) 
	{
		input[i] = b.Get(i) == 0 ? 0.f : 1.f;
		output[i] = m.index == i ? 1.f : 0.f;
	}
}

void Extractor::ExtractForAssignment
(
	  Mutation m
	, Board&   b
	, float*   input
	, float*   output
)
{
	char output[DEPENDENCYSIZE + SIZE];
	bool berOptions[SIZE];
	for (int j = 0; j < 3; j++) 
	{
		for (int k = 0; k < SIZE-1; k++) 
		{
			SetBool(berOptions, *(b.dependencies[m.value][j][k]));
			for (int i = 0; i < SIZE; i++) 
			{
				input[(j*(SIZE-1) + k)*SIZE + i] = berOptions[i] ? 1.f : 0.f;
			}
		}
	}
	
	SetBool(berOptions, m.value);
	for (int i = 0; i < SIZE; i++) 
	{
		output[i] = berOptions[i] ? 1.f : 0.f;
	}
}

int Extractor::GetExtractions(
	  Board&   b 
	, Mutation muts[BOARDSIZE]
	, float*   inputPols
	, float*   outputPols
	, float*   inputAsns
	, float*   outputAsns
)
{
	int c = 0;
		
	for(int j = 0; j < BOARDSIZE; j++)
	{
		if(b[j]) c++;
	}
	Solver::Solve(b, muts);
	for(int j = 0; j < c; j++)
	{
		// undo solution
		b[muts[j].index] = 0;
	}
	
	for(int j = 0; j < c; j++)
	{
		Extractor::ExtractForPolicy
		(
			  muts[j]
			, b
			, &inputPols[j*BOARDSIZE]
			, &outputPols[j*BOARDSIZE]
		);
		Extractor::ExtractForAssignment
		(
			  muts[j]
			, b
			, &inputAsns[j*3*(SIZE-1)*SIZE]
			, &outputAsns[j*SIZE]
		);
		b.Apply(muts[j]);
	}
	
	return c;
}
