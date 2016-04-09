#ifndef BUILD
#define BUILD

#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <list>
#include <iostream>
#include <iomanip>

#define boxSize 3
#define size ( boxSize * boxSize )
#define colSize ( boxSize * boxSize )
#define rowSize ( boxSize * boxSize )
#define boardSize ( size * size )

#include "Board.h"
#include "Solver.h"
#include "Destructifier.h"
#include "Puzzle Generator.h"

#endif