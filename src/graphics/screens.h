// =============================================================================
//  screens.h  -  The screens of the program
//
//  Every screen has two functions:
//    update...  reads the keyboard and the mouse and changes the variables
//    draw...    only draws
// =============================================================================
#ifndef SCREENS_H
#define SCREENS_H

// The screens. Only one is shown at a time.
const int SCREEN_MENU = 0;
const int SCREEN_SIZE_SELECT = 1;
const int SCREEN_HELP = 2;
const int SCREEN_ABOUT = 3;
const int SCREEN_PLAYING = 4;
const int SCREEN_EXIT = 5;   // the program closes

// Created in main.cpp, used by all screens.
extern int currentScreen;  // which screen is shown right now
extern float appTime;      // seconds since the start (for the animations)

// Main menu (menu_screen.cpp)
void updateMenuScreen();
void drawMenuScreen();

// Choosing the maze size (size_screen.cpp)
void openSizeScreen();
void updateSizeScreen();
void drawSizeScreen();

// "How to play" and "About" (info_screens.cpp)
void updateInfoScreen();
void drawHelpScreen();
void drawAboutScreen();

// The game and the victory window (game_screen.cpp)
void startPlaying(int size);
void stopPlaying();
void updateGameScreen(float deltaTime);
void drawGameScreen();

#endif // SCREENS_H
