#ifndef BUILD
#define BUILD

#include <time.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <list>
#include <stack>
#include <iostream>
#include <iomanip>
#include <fstream> 

#define boxSize 3
#define size ( boxSize * boxSize )
#define colSize ( boxSize * boxSize )
#define rowSize ( boxSize * boxSize )
#define boardSize ( size * size )

//For Dependancy
#define ROW 0
#define COL 1
#define BOX 2

#include "Board.h"
#include "Solver.h"
#include "Destructifier.h"
#include "Puzzle Generator.h"

#endif