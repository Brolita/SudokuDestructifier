#include "Build Settings.h"


int Extractor::Extract
(
	  Board& b
	, Mutation muts[BOARDSIZE]
	, double* inputPolicy
	, double* outputPolicy
	, double* inputAssignment
	, double* outputAssignment
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
		Extractor::ExtractForPolicy(muts[j], b, inputPolicy, outputPolicy);
		Extractor::ExtractForAssignment(muts[j], b, inputAssignment, outputAssignment);
		b.Apply(muts[j]);
	}
	
	return c;
}

void Extractor::ExtractForPolicy(Mutation m, Board& b, double* input, double* output)
{	
	int mbfGoal = 0;
	for (int v = 0; v < SIZE; v++)
		mbfGoal += Solver::CanPlace(v+1, b, m.index);
	
	for (int i = 0; i < BOARDSIZE; i++) 
	{
		input[i] = b.Get(i) != 0 ? 1.f : 0.f;
	}
	
	for (int i = 0; i < BOARDSIZE; i++) 
	{
		int mbf = 0;
		for (int v = 0; v < SIZE; v++) {
			mbf += Solver::CanPlace(v+1, b, i) && (b[i] == 0);
		}
		
		output[i] = mbf == mbfGoal ? 1.f : 0.f;
	}
}

void Extractor::ExtractForAssignment(Mutation m, Board& b, double* input, double* output)
{
	bool berOptions[SIZE];
	
	int c = 0;
	for (int j = 0; j < 3; j++) 
	{
		for (int k = 0; k < SIZE-1; k++) 
		{
			SetBool(berOptions, *(b.dependencies[m.index][j][k]));
			for (int i = 0; i < SIZE; i++) 
			{
				input[c] = berOptions[i] ? 1.f : 0.f;
				c++;
			}
		}
	}
	
	for (int i = 0; i < SIZE; i++)
	{
		output[i] = Solver::CanPlace(i + 1, b, m.index) ? 1.f : 0.f; 
	}
}

void Extractor::ExtractForPolicy(Mutation m, Board& b, std::ofstream* ofs) 
{	
	int mbfGoal = 0;
	for (int v = 0; v < SIZE; v++)
		mbfGoal += Solver::CanPlace(v+1, b, m.index);
	
	for (int i = 0; i < BOARDSIZE; i++) 
	{
		*ofs << (b.Get(i) != 0) << " ";
	}
	
	*ofs << '\t';
	
	for (int i = 0; i < BOARDSIZE; i++) 
	{
		int mbf = 0;
		for (int v = 0; v < SIZE; v++) {
			mbf += Solver::CanPlace(v+1, b, i) && (b[i] == 0);
		}
		
		*ofs << (mbf == mbfGoal) << " ";
	}
	
	*ofs << std::endl;
}

void Extractor::ExtractForAssignment(Mutation m, Board& b, std::ofstream* ofs) 
{
	bool berOptions[SIZE];
	
	int c = 0;
	for (int j = 0; j < 3; j++) 
	{
		for (int k = 0; k < SIZE-1; k++) 
		{
			SetBool(berOptions, *(b.dependencies[m.index][j][k]));
			for (int i = 0; i < SIZE; i++) 
			{
				*ofs << berOptions[i] << " ";
				c++;
			}
		}
	}
	
	*ofs << '\t';
	
	for (int i = 0; i < SIZE; i++)
	{
		*ofs << Solver::CanPlace(i + 1, b, m.index) << " ";
	}
	
	*ofs << std::endl;
}

