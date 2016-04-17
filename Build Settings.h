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
#include <signal.h> 
#include <sys/types.h>

#include "vt100.h"
//For advance output
//	Can be OS locked if run on SOME old windows / non-standard linux

#define boxSize 3
#define size ( boxSize * boxSize )
#define colSize ( boxSize * boxSize )
#define rowSize ( boxSize * boxSize )
#define boardSize ( size * size )
#define outputSize ( size + boxSize + 1 )
#define board1y 4
#define board2y (board1y + outputSize + 2)
#define board3y (board2y + outputSize + 2)

//coreeeeeeeeeeey
#define BOXSIZE boxSize
#define SIZE size
#define COLSIZE colSize
#define ROWSIZE rowSize
#define BOARDSIZE boardSize
#define OUTPUTSIZE outputSize
#define BOARD1Y board1y
#define BOARD2Y board2y
#define BOARD3Y board3y

//For Dependancy
#define ROW 0
#define COL 1
#define BOX 2

#include "Board.h"
#include "Solver.h"
#include "Destructifier.h"
#include "Puzzle Generator.h"

#endif