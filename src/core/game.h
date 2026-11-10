// =============================================================================
//  game.h  -  Game rules: moving the player, items, score, hints
//
//  This file does NOT use cin/cout. The user interface calls these functions
//  and shows the results on the screen.
// =============================================================================
#ifndef GAME_H
#define GAME_H

#include <string>

#include "maze.h"

// Directions
const int DIR_NONE = 0;
const int DIR_UP = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;
const int DIR_RIGHT = 4;

// What can happen when the player tries to move (returned by movePlayer)
const int MOVE_OK = 0;           // a normal step
const int MOVE_WALL = 1;         // a wall is in the way - the player did not move
const int MOVE_OUTSIDE = 2;      // the player tried to walk off the map
const int MOVE_COIN = 3;         // stepped on a data chip
const int MOVE_DOOR = 4;         // walked through a secret door
const int MOVE_EXIT = 5;         // reached the exit - victory!
const int MOVE_GAME_OVER = 6;    // the game is already won, no more moves
const int MOVE_NO_DIRECTION = 7; // DIR_NONE was given

// The player. The variables are created in game.cpp.
extern int playerRow;     // current row (0 = top)
extern int playerCol;     // current column (0 = left)
extern int moves;         // steps made
extern int coins;         // data chips collected
extern int doorsUsed;     // how many times a secret door was used
extern int hintsUsed;     // how many hints were asked for
extern int optimalMoves;  // length of the shortest route at the start
extern bool isWon;        // true when the exit is reached
extern bool** visited;    // visited[row][col] = true if the player was there

// ---- Checking user input -------------------------------------------------------

// true if the size is from 10 to 50.
bool isValidMazeSize(int size);

// true if the text is a whole number like "25", " 42 " or "-7".
// false for letters, "12abc", "3.5", empty text or a very long number.
bool isWholeNumber(std::string text);

// Turns the text into a number. Use it only after isWholeNumber said true.
int textToNumber(std::string text);

// ---- Starting and ending a game ------------------------------------------------

// Creates a random maze of this size and starts a new game in it.
void startNewGame(int size, unsigned int seed);

// Starts a game in the maze that is already created (used by the tests).
void startGame();

// Frees all the memory of the game (the maze and "visited").
void endGame();

// ---- Playing ---------------------------------------------------------------------

// How the row / the column changes in this direction (Up -> row - 1 ...).
int rowChange(int direction);
int colChange(int direction);

// Tries to move the player one tile. Returns one of the MOVE_... numbers.
int movePlayer(int direction);

// The direction of the next step on the shortest way to the exit.
int findHintDirection();

// Same as findHintDirection, but counts the hint (hints cost points).
int useHint();

// How many steps the player is away from the exit right now.
int getStepsToExit();

// The score: chips give points, extra moves and hints take points away.
int calculateScore();

// A funny title for the player, depending on the score.
std::string getRankTitle(int score);

#endif // GAME_H
