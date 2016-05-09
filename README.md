# SudokuDestructifier

This tool uses neural networks to solve self-generated sudoku puzzles. 

To build the application, run "./compile.sh" from the linux shell
 * To solve a random puzzle, run the application as you first downloaded it. Change BOXSIZE in "Build Settings.h" and the two paths from which the neural networks read data in "main.cpp" as desired, but note that this version only contains good neural network data for 2x2 puzzles. 
 * To generate new neural network data, comment out the code labeled "solve a random puzzle" in "main.cpp" and uncomment the code labeled "generate data and train networks". Set BOXSIZE to the desired value in "Build Settings.h". 

To run the application, run "./run.sh" from the linux shell. 
