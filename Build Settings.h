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
//For advanced output
//	Can be OS locked if run on SOME old windows / non-standard linux

#define BOXSIZE 3
#define SIZE ( BOXSIZE * BOXSIZE )
#define COLSIZE ( BOXSIZE * BOXSIZE )
#define ROWSIZE ( BOXSIZE * BOXSIZE )
#define BOARDSIZE ( SIZE * SIZE )
#define OUTPUTSIZEY ( SIZE + BOXSIZE + 1 )
#define OUTPUTSIZEX ( 2*BOXSIZE + 2*SIZE + 2 )
#define BOARD1Y 3
#define BOARD2Y (BOARD1Y + OUTPUTSIZEY + 2)
#define BOARD3Y (BOARD2Y + OUTPUTSIZEY + 2)
#define BOARD1X 3
#define BOARD2X (BOARD1X + OUTPUTSIZEX + 2)
#define NULLHI BOARDSIZE + 1

//For Generator
#define GENERATORS 10
#define TRAINERS 10

//For Dependancy
#define ROW 0
#define COL 1
#define BOX 2

#include "Board.h"
#include "Solver.h"
#include "Destructifier.h"
#include "Puzzle Generator.h"
#include "Extractor.h"

void SetBool(bool a[SIZE], char v);

#endif