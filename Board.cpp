#include "Build Settings.h"

//#define newline cursor_down(1) << cursor_left( OUTPUTSIZEX )
#define newline std::endl

Board::Board()
{ 
	memset(data, 0, BOARDSIZE);
	for(int i = 0; i < BOARDSIZE; i++)
		this->Dependency(i, dependencies[i]);
}

Board::Board(const Board& b) 
{
	*this = b;
}

int Board::Index(int x, int y)
{
	return x + y * COLSIZE;
}

int Board::Index(int bx, int by, int ox, int oy)
{
	return ((bx * BOXSIZE) + ox) + (((by * BOXSIZE) + oy) * SIZE);
}

Board& Board::operator= (const Board& b) {
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < SIZE-1; k++) {
				dependencies[i][j][k] = b.dependencies[i][j][k];
			}
		}
	}

	for (int i = 0; i < BOARDSIZE; i++) {
		data[i] = b.data[i];
	}

	return *this;
}

bool operator==(Board& b1, Board& b2)
{
	for(int i = 0; i < BOARDSIZE; i++)
	{
		if(b1[i] != b2[i])
			return 0;
	}
	return 1;
}

bool operator!=(Board& b1, Board& b2)
{
	return !(b1 == b2);
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

int Board::Count() {
	int c = 0;
	for(int i = 0; i < BOARDSIZE; ++i) {
		c += (bool)data[i];
	}
	return c;
}

Board& Board::operator= (Board b) {
	std::cout << "hi there\n";


	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < SIZE-1; k++) {
				dependencies[i][j][k] = b.dependencies[i][j][k];
			}
		}
	}

	for (int i = 0; i < BOARDSIZE; i++) {
		data[i] = b.data[i];
	}

	return *this;
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
 
void Board::Dependency(int index, char* v[3][SIZE - 1])
{
	int argBoxRow[BOXSIZE];
	int argBoxCol[BOXSIZE];
	int argRow[BOXSIZE];
	int argCol[BOXSIZE];
	
	for(int i = 0; i < BOXSIZE; i++)
	{
		argBoxCol[i] = i;
		argBoxRow[i] = i;
		argRow[i] = i;
		argCol[i] = i;
	}
	
	int x = index % SIZE, y = index / SIZE;
	
	int bx = x / BOXSIZE, by = y / BOXSIZE;
	int ox = x % BOXSIZE, oy = y % BOXSIZE;
	
	Swap(&argBoxRow[0], &argBoxRow[bx]);
	Swap(&argBoxCol[0], &argBoxCol[by]);
	Swap(&argRow[0], 	&argRow[ox]);
	Swap(&argCol[0], 	&argCol[oy]);
	
	for(int i = 1; i < SIZE; i++)
	{
		int b = i % BOXSIZE;
		int a = i / BOXSIZE;

		v[ROW][i-1] = &Get(argBoxRow[a], argBoxCol[0], argRow[b], argCol[0]);
		v[COL][i-1] = &Get(argBoxRow[0], argBoxCol[a], argRow[0], argCol[b]);
		v[BOX][i-1] = &Get(argBoxRow[0], argBoxCol[0], argRow[a], argCol[b]);
	}
}

void Board::SolvedPositions(bool o[BOARDSIZE])
{
	for(int i = 0; i < BOARDSIZE; i++)
		o[i] = data[i] != 0;
}

void Board::Clear()
{
	memset(data, 0, BOARDSIZE);
}

void Board::Apply(Mutation m)
{
	data[m.index] = m.value;
}

bool Board::isValid()
{
	for (int i=0; i<BOXSIZE; i++)
	{
		for (int j=0; j<BOXSIZE; j++)
		{
			int val = this->Index(i,j,j,i);
			//std::cout << i << " " << j << " " << val << std::endl;
			//char* depend[3][SIZE-1];
			//this->Dependency(val,depend);
			for (int k=0; k<3; k++) 
			{
				bool check[SIZE];
				for (int l=0; l<SIZE-1; l++)
				{
					//std::cout << char(*dependencies[val][k][l] + '0') << " at " << k << " " << l << std::endl;
					if (*dependencies[val][k][l] == 0) {
						continue;
					}
					else if (!check[*dependencies[val][k][l]-1])
					{
						check[*dependencies[val][k][l]-1] = true;
					}
					else
					{
						//std::cout << "fails here1" << std::endl;
						return false;
					}
				}
				if (!check[this->Get(val)-1]) 
				{
					check[this->Get(val)-1] = true;
				}
				else 
				{
					//std::cout << "fails here2" << std::endl;
					return false;
				}
				for (int l=0; l<SIZE; l++) {
					check[l] = false;
				}
				//for (int l=0; l<SIZE; l++) 
				//{
					//std::cout << "fails here3" << std::endl;
					//if (!check[l]) return false;
					//check[l] = false;
				//}
			}
		}
	}
	
	/*
	for (int i=0; i<BOARDSIZE; i++) {
		//check box
		for (int j=0; j<SIZE; j++){
			int val = (i/SIZE/BOXSIZE)*BOXSIZE*SIZE + ((i%SIZE)/BOXSIZE)*BOXSIZE;
			val = val + j%BOXSIZE + (j/BOXSIZE)*SIZE;
			if (val == i) continue;
			if (data[val] == data[i]) {
				return false;
			}
		}
		//check row
		for (int j=0;j<COLSIZE;j++){
			int val = i/SIZE*SIZE + j;
			if (val == i) continue;
			if (data[val] == data[i]) {
				return false;
			}
		}
		//check column
		for (int j=0;j<COLSIZE;j++){
			int val = j*SIZE + i%SIZE;
			if (val == i) continue;
			if (data[val] == data[i]) {
				return false;
			}
		}
	}
	*/
	return true;
}

void Board::printBoard()
{
	std::string horizontal = std::string(2*BOXSIZE + 2*SIZE + 1, '-') + std::string(" ");
	int count = 0;
	for (int i=0; i<horizontal.length()-2*count; i++) 
	{
		if (i%(BOXSIZE*2) == 0) 
		{
			horizontal[i+2*count] = '+';
			count++;
		}
	}
	std::cout << horizontal << newline;
	for (int y = 0; y<SIZE; y++)
	{
		std::cout << "| ";
		for (int x = 0; x<SIZE; x++) 
		{
			char print = this->Get(x,y);
			if(print == 0)
				print = ' ';
			else if(print < 10)
				print += '0';
			else 
				print += 'a' - 10;
			std::cout << print << " ";
			if ((x+1)%BOXSIZE == 0) 
				std::cout << "| "; 	
		}
		std::cout << newline;
		if ( (y+1)%BOXSIZE == 0) 
		{
			std::cout << horizontal << newline;
		}
	}
	//std::cout << TopBottom << std::endl;
}


