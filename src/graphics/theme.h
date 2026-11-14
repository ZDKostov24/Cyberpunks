// =============================================================================
//  theme.h  -  Colors and window size of the graphical version
// =============================================================================
#ifndef THEME_H
#define THEME_H

#include "raylib.h"

// The game is designed for this size. Bigger screens are scaled (see main.cpp).
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;

// Select Red, Blue, or Yellow from the main menu with left/right arrows.
enum ThemeChoice { THEME_RED, THEME_BLUE, THEME_YELLOW, THEME_COUNT };
extern int currentTheme;
void setTheme(int theme);
const char* getThemeName();
enum PlayerColorChoice { PLAYER_CYAN, PLAYER_PINK, PLAYER_GREEN, PLAYER_COLOR_COUNT };
extern int currentPlayerColor;
void setPlayerColor(int color);
const char* getPlayerColorName();

// Shared palette colors. setTheme() updates these before the next frame.
extern Color COLOR_BACKGROUND;
extern Color COLOR_FLOOR;
extern Color COLOR_WALL;
extern Color COLOR_NEON_RED;
extern Color COLOR_DARK_RED;
extern Color COLOR_PLAYER;
extern Color COLOR_VISOR;
extern Color COLOR_EXIT;
extern Color COLOR_DOOR;
extern Color COLOR_COIN;
extern Color COLOR_TEXT;
extern Color COLOR_TEXT_DIM;
extern Color COLOR_PANEL;

#endif // THEME_H
