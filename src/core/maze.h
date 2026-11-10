// =============================================================================
//  maze.h  -  The maze: creating it, generating it, finding the shortest path
//
//  This file does NOT use cin/cout. It only stores and calculates data.
// =============================================================================
#ifndef MAZE_H
#define MAZE_H

// The allowed maze sizes (from the task).
const int MIN_MAZE_SIZE = 10;
const int MAX_MAZE_SIZE = 50;

// Every tile of the maze is a number:
const int TILE_WALL = 0;  // the player cannot walk here
const int TILE_PATH = 1;  // free corridor
const int TILE_DOOR = 2;  // secret door - a wall the player CAN pass (a shortcut)
const int TILE_COIN = 3;  // data chip - gives bonus points
const int TILE_EXIT = 4;  // the way out (bottom-right corner)

// The maze itself. The variables are created in maze.cpp;
// "extern" means that the other files can use them too.
extern int mazeSize;  // number of rows = number of columns
extern int** maze;    // dynamic 2D array: maze[row][col] = tile number

// Creates a size x size maze full of walls (memory is taken with new).
void createMaze(int size);

// Frees the memory of the maze (with delete).
void deleteMaze();

// Fills the maze with a new random labyrinth.
// The same seed always gives the same maze (useful for the tests).
void generateMaze(unsigned int seed);

// true if (row, col) is inside the maze.
bool isInsideMaze(int row, int col);

// true if the player can stand on (row, col).
bool isWalkable(int row, int col);

// How many tiles of this type are in the maze.
int countTiles(int tile);

// Number of steps from (row, col) to the exit on the shortest path.
// Returns -1 if the exit cannot be reached.
int findShortestPathLength(int row, int col);

#endif // MAZE_H
