#include "Build Settings.h"

Board::Board()
{ 
	memset(data, 0, boardSize);
	for(int i = 0; i < boardSize; i++)
		this->Dependency(i, dependencies[i]);
}

int Board::Index(int x, int y)
{
	return x + y * colSize;
}

int Board::Index(int bx, int by, int ox, int oy)
{
	return ((bx * boxSize) + ox) + (((by * boxSize) + oy) * size);
}

char& Board::Get(int index)
{
	return data[index];
}

char& Board::Get(int x, int y)
{
	return data[Index(x, y)];
}

char& Board::Get(int bx, int by, int ox, int oy)
{
	return data[Index(bx, by, ox, oy)];
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
 
void Board::Dependency(int index, char* v[3][size - 1])
{
	int argBoxRow[boxSize];
	int argBoxCol[boxSize];
	int argRow[boxSize];
	int argCol[boxSize];
	
	for(int i = 0; i < boxSize; i++)
	{
		argBoxCol[i] = i;
		argBoxRow[i] = i;
		argRow[i] = i;
		argCol[i] = i;
	}
	
	int x = index % size, y = index / size;
	
	int bx = x / boxSize, by = y / boxSize;
	int ox = x % boxSize, oy = y % boxSize;
	
	Swap(&argBoxRow[0], &argBoxRow[bx]);
	Swap(&argBoxCol[0], &argBoxCol[by]);
	Swap(&argRow[0], 	&argRow[ox]);
	Swap(&argCol[0], 	&argCol[oy]);
	
	for(int i = 1; i < size; i++)
	{
		int b = i % boxSize;
		int a = i / boxSize;

		v[ROW][i-1] = &Get(argBoxRow[a], argBoxCol[0], argRow[b], argCol[0]);
		v[COL][i-1] = &Get(argBoxRow[0], argBoxCol[a], argRow[0], argCol[b]);
		v[BOX][i-1] = &Get(argBoxRow[0], argBoxCol[0], argRow[a], argCol[b]);
	}
}

void Board::SolvedPositions(bool o[boardSize])
{
	for(int i = 0; i < boardSize; i++)
		o[i] = data[i] != 0;
}

void Board::Clear()
{
	memset(data, 0, boardSize);
}

void Board::Apply(Mutation m)
{
	data[m.index] = m.value;
}

bool Board::isValid()
{
	//for (int i=0; i<)
	return true;
}

void Board::printBoard()
{
	int maxLen = (int) log10((double)size) + 1;
	std::string horizontal = std::string(2*boxSize + size*(maxLen+1) + 1, '-');
	int count = 0;
	for (int i=0; i<horizontal.length()-2*count; i++) {
		if (i%(boxSize*(maxLen+1)) == 0) {
			horizontal[i+2*count] = '+';
			count++;
		}
	}
	std::cout << horizontal << std::endl;
	for (int y = 0; y<size; y++){
		std::cout << "| ";
		for (int x = 0; x<size; x++) {
			char print = this->Get(x,y);
			if(print == 0)
				print = ' ';
			else if(print < 10)
				print += '0';
			else 
				print += 'a' - 10;
			std::cout << std::setw(maxLen)
				<< print << " ";
			if ((x+1)%boxSize == 0) {
				std::cout << "| ";
			}
		}
		std::cout << std::endl;
		if ( (y+1)%boxSize == 0) {
			std::cout << horizontal << std::endl;
		}
	}
	//std::cout << TopBottom << std::endl;
}


