// =============================================================================
//  theme.cpp - Selectable color palettes for the graphical version
// =============================================================================
#include "theme.h"

int currentTheme = THEME_RED;
int currentPlayerColor = PLAYER_CYAN;

Color COLOR_BACKGROUND;
Color COLOR_FLOOR;
Color COLOR_WALL;
Color COLOR_NEON_RED;
Color COLOR_DARK_RED;
Color COLOR_PLAYER;
Color COLOR_VISOR;
Color COLOR_EXIT;
Color COLOR_DOOR;
Color COLOR_COIN;
Color COLOR_TEXT;
Color COLOR_TEXT_DIM;
Color COLOR_PANEL;

void setTheme(int theme)
{
    if (theme < 0 || theme >= THEME_COUNT) theme = THEME_RED;
    currentTheme = theme;

    COLOR_BACKGROUND = { 7, 5, 9, 255 };
    COLOR_FLOOR = { 14, 10, 16, 255 };
    COLOR_WALL = { 42, 7, 14, 255 };
    COLOR_DARK_RED = { 75, 10, 22, 255 };
    COLOR_VISOR = { 0, 35, 46, 255 };
    COLOR_EXIT = { 255, 235, 40, 255 };
    COLOR_DOOR = { 255, 60, 220, 255 };
    COLOR_COIN = { 255, 200, 40, 255 };
    COLOR_TEXT = { 242, 240, 245, 255 };
    COLOR_TEXT_DIM = { 155, 160, 175, 255 };
    COLOR_PANEL = { 14, 12, 20, 235 };

    if (theme == THEME_BLUE)
    {
        COLOR_NEON_RED = { 45, 145, 255, 255 };
        COLOR_PLAYER = { 70, 240, 255, 255 };
        COLOR_DARK_RED = { 12, 44, 100, 255 };
        COLOR_WALL = { 7, 20, 48, 255 };
        COLOR_FLOOR = { 8, 14, 28, 255 };
        COLOR_BACKGROUND = { 4, 7, 18, 255 };
        COLOR_VISOR = { 0, 48, 78, 255 };
        COLOR_TEXT_DIM = { 115, 155, 195, 255 };
        COLOR_PANEL = { 6, 14, 30, 235 };
    }
    else if (theme == THEME_YELLOW)
    {
        COLOR_NEON_RED = { 255, 194, 35, 255 };
        COLOR_PLAYER = { 255, 239, 125, 255 };
        COLOR_DARK_RED = { 112, 69, 8, 255 };
        COLOR_WALL = { 48, 31, 7, 255 };
        COLOR_FLOOR = { 25, 19, 8, 255 };
        COLOR_BACKGROUND = { 12, 9, 4, 255 };
        COLOR_VISOR = { 83, 58, 5, 255 };
        COLOR_EXIT = { 255, 250, 190, 255 };
        COLOR_COIN = { 255, 226, 95, 255 };
        COLOR_DOOR = { 255, 139, 45, 255 };
        COLOR_TEXT_DIM = { 190, 158, 90, 255 };
        COLOR_PANEL = { 25, 18, 7, 235 };
    }
    else
    {
        COLOR_NEON_RED = { 255, 26, 46, 255 };
        COLOR_PLAYER = { 0, 229, 255, 255 };
    }
    setPlayerColor(currentPlayerColor);
}

void setPlayerColor(int color)
{
    if (color < 0 || color >= PLAYER_COLOR_COUNT) color = PLAYER_CYAN;
    currentPlayerColor = color;
    if (color == PLAYER_PINK) COLOR_PLAYER = { 255, 90, 210, 255 };
    else if (color == PLAYER_GREEN) COLOR_PLAYER = { 100, 255, 130, 255 };
    else COLOR_PLAYER = { 0, 229, 255, 255 };
}

const char* getPlayerColorName()
{
    if (currentPlayerColor == PLAYER_PINK) return "PINK";
    if (currentPlayerColor == PLAYER_GREEN) return "GREEN";
    return "CYAN";
}

const char* getThemeName()
{
    if (currentTheme == THEME_BLUE) return "BLUE";
    if (currentTheme == THEME_YELLOW) return "YELLOW";
    return "RED";
}

namespace
{
    struct InitialTheme
    {
        InitialTheme() { setTheme(THEME_RED); }
    } initialTheme;
}
