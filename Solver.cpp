#include "Build Settings.h"

bool Solver::CanPlace(char value, char* dependency[3][size-1]) 
{	
	for(int j = 0; j < 3; j++)
	{
		for(int k = 0; k < size - 1; k++)
		{
			if((*(dependency[j][k])) == value) {
				return false;
			}
		}
	}
	return true;
}


