// =============================================================================
//  theme.h  -  Colors and window size of the graphical version
// =============================================================================
#ifndef THEME_H
#define THEME_H

#include "raylib.h"

// The game is designed for this size. Bigger screens are scaled (see main.cpp).
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;

// Neon cyberpunk colors { red, green, blue, transparency }
const Color COLOR_BACKGROUND = { 7, 5, 9, 255 };       // almost black
const Color COLOR_FLOOR = { 14, 10, 16, 255 };    // corridor floor
const Color COLOR_WALL = { 42, 7, 14, 255 };     // inside of a wall
const Color COLOR_NEON_RED = { 255, 26, 46, 255 };   // main neon color
const Color COLOR_DARK_RED = { 138, 0, 16, 255 };
const Color COLOR_PLAYER = { 0, 229, 255, 255 };   // cyan
const Color COLOR_VISOR = { 0, 70, 90, 255 };     // dark inside of the player
const Color COLOR_EXIT = { 255, 235, 40, 255 };  // yellow
const Color COLOR_DOOR = { 255, 60, 220, 255 };  // magenta
const Color COLOR_COIN = { 255, 200, 40, 255 };  // gold
const Color COLOR_TEXT = { 255, 227, 230, 255 }; // light pink-white
const Color COLOR_TEXT_DIM = { 150, 105, 112, 255 }; // grey-pink
const Color COLOR_PANEL = { 18, 8, 12, 235 };     // dark panels

#endif // THEME_H
