// =============================================================================
//  console_ui.h  -  The TEXT version: menus, drawing the maze, reading keys
//
//  This is the only part of the text version that uses cin / cout.
// =============================================================================
#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <string>

// What the player can do with the keyboard during a game.
const int COMMAND_UP = 1;       // same numbers as DIR_UP ... DIR_RIGHT in game.h
const int COMMAND_DOWN = 2;
const int COMMAND_LEFT = 3;
const int COMMAND_RIGHT = 4;
const int COMMAND_HINT = 5;
const int COMMAND_RESTART = 6;
const int COMMAND_QUIT = 7;
const int COMMAND_UNKNOWN = 8;

// Turns on colors in the Windows console.
void prepareConsole();

void clearScreen();

// Menus and screens
void showMainMenu();
void showHelp();
void showAbout();
void showVictoryScreen();

// Asks for a whole number again and again, until it is between min and max.
int readNumberInRange(std::string question, int minValue, int maxValue);

// Waits for one key (arrows, WASD, H, R, Q) - no Enter needed.
// Returns one of the COMMAND_... numbers.
int readCommand();

// Waits until any key is pressed.
void waitForAnyKey();

// Draws the maze, the player, the status line and a message.
// If hintDirection is not DIR_NONE, an arrow shows the next step.
void drawGame(std::string message, int hintDirection);

// The text to show after a move, e.g. "+1 data chip!".
std::string describeMoveResult(int result);

#endif // CONSOLE_UI_H
