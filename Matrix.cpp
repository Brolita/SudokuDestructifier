#include "Build Settings.h"

void Matrix::Initalize(int _x, int _y, float _i) 
{
	x = _x;
	y = _y;
	v = (float**)malloc(_x * sizeof(float*));
	
	for(int i = 0; i < x; i++)
	{
		v[i] = (float*)malloc(_y * sizeof(float));
		for(int j = 0; j < y; j++)
		{
			v[i][j] = _i;
		}
	}
}

Matrix::Matrix(const Matrix& m) 
{
	*this = m;
}

Matrix::~Matrix()
{
	if(v) 
	{
		for(int i = 0; i < x; i++)
		{
			free(v[i]);
		}
		free(v);
	}
}
