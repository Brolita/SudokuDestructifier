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
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include "vt100.h"
//For advanced output
//	Can be OS locked if run on SOME old windows / non-standard linux

#define lineprint std::cout << __LINE__ << std::endl

#define BOXSIZE 2
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

//For Destructifier 
#define PRUNE_POLICY 0.3
#define PRUNE_ASSIGN 0.4
#define VALIDATE_ASSIGN 0.7
#define DEPENDENCYSIZE 3 * (SIZE-1) * SIZE

//For Testing
#define MAXTIME 10 //in seconds
#define NPOLICY 10
#define NASSIGN 10
#define TOTALPOLICY 1000
#define TOTALASSIGN 100
#define NPUZZLES 10
#define STRBUF 200
 
//For Generator
#define GENERATORS 10
#define TRAINERS 10

//For Dependancy
#define ROW 0
#define COL 1
#define BOX 2

//For Neural Net
#define P_ETA 1
#define P_EPS .3
#define A_ETA 1
#define A_EPS .3
#define P_HIDDEN 32
#define A_HIDDEN 36

#include "Board.h"
#include "Solver.h"
#include "Puzzle Generator.h"
#include "Extractor.h"
#include "Matrix.h"
#include "Neural Net.h"
#include "Dummy ANet.h"
#include "Dummy PNet.h"
#include "Destructifier.h"

void SetBool(bool a[SIZE], char v);

#endif