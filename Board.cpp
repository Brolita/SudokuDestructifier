#include "Build Settings.h"

//For Dependancy
#define ROW 0
#define COL 1
#define BOX 2

Board::Board()
{ }

int Board::Index(int x, int y)
{
	return y * colSize + x;
}

int Board::Index(int ox, int oy, int bx, int by)
{
	return (oy * boxSize + by) * size + ox * boxSize + bx;
}

char& Board::Get(int index)
{
	return data[index];
}

char& Board::Get(int x, int y)
{
	return data[Index(x, y)];
}

char& Board::Get(int ox, int oy, int bx, int by)
{
	return data[Index(ox, oy, bx, by)];
}

char& Board::operator[] (int index)
{
	return data[index];
}

/* Dependency for an index
 *  	         
 * Example, dependencies for D
 *
 *  	A B  	               
 *  	C D G H   
 *  	  E 	  
 *  	  F	  	  
 * 
 * Row and column "swaps" can be used to normalize into
 * swaps apply to every box to make the operation quicker (doesn't effect outcome)
 *
 * 	D C H G
 *  B A     and will return [C, G, H] [B, F, E] [C, B, A]
 *  F
 *  E
 *
 * Also box swaping to change (on E)
 *
 *      A B        A B           E F C D
 *  C D E F   --\  E F C D  --\  A B
 *      G     --/  G        --/  H
 *      H          H             G
 *  						
 * in a larger box:
 *
 * A B C J K L M N O			
 * D E F    			 		
 * G H I						
 * P
 * Q
 * R		will return [B, C, J, K, K, L, M, N, O], [D, G, P, Q, R, S, T, U], [B, C, D, E, F, G, H, I]
 * S
 * T
 * U
 * 
 * Note: char *** , the chars are all returned ar pointers
 */
 
// a good shorthand
template <class T>
void Swap(T* a, T* b)
{
	T n = *a;
	*a = *b;
	*b = n;
}
 
char*** Board::Dependency(int index)
{
	char*** v = (char***) new char*[3][size - 1];

	int argBoxRow[boxSize];
	int argBoxCol[boxSize];
	int argRow[boxSize];
	int argCol[boxSize];
	
	for(int i = 0; i < size; i++)
	{
		argBoxCol[i] = i;
		argBoxRow[i] = i;
		argRow[i] = i;
		argCol[i] = i;
	}
	
	int x = index % size, y = index / size;
	
	int ox = x / boxSize, oy = y / boxSize;
	int bx = x % boxSize, by = y % boxSize;
	
	Swap(&argBoxRow[0], &argBoxRow[ox]);
	Swap(&argBoxCol[0], &argBoxCol[oy]);
	Swap(&argRow[0], 	&argRow[bx]);
	Swap(&argCol[0], 	&argCol[by]);
	
	for(int i = 1; i < size; i++)
	{
		int b = i % boxSize;
		int a = i / boxSize;
		
		v[ROW][i-1] = &data[argBoxCol[a], argBoxRow[0], argCol[0], argRow[b]];
		v[COL][i-1] = &data[argBoxCol[0], argBoxRow[a], argCol[b], argRow[0]];
		v[BOX][i-1] = &data[argBoxCol[0], argBoxRow[0], argCol[a], argRow[b]];
	}
	
	return v;
}

char*** Board::operator() (int index)
{
	return Dependency(index);
}
