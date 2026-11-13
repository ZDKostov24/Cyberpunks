// =============================================================================
//  menu_screen.cpp  -  The main menu
// =============================================================================
#include <string>

#include "fonts.h"
#include "game.h"
#include "maze_renderer.h"
#include "screens.h"
#include "theme.h"
#include "widgets.h"

using namespace std;

const int MENU_ITEM_COUNT = 4;
string menuLabels[MENU_ITEM_COUNT] = { "NEW GAME", "HOW TO PLAY", "ABOUT", "EXIT" };

int selectedMenuItem = 0; // which button is lit when using the keyboard

// All menu buttons have the same size and are under each other.
const float MENU_BUTTON_X = 460.0f;
const float MENU_BUTTON_WIDTH = 360.0f;
const float MENU_BUTTON_HEIGHT = 58.0f;

// The y position of menu button number i (0 = top button).
float menuButtonY(int i)
{
    return 390.0f + i * 74.0f;
}

// What happens when menu button number i is chosen.
void openMenuItem(int i)
{
    if (i == 0) openSizeScreen();
    if (i == 1) currentScreen = SCREEN_HELP;
    if (i == 2) currentScreen = SCREEN_ABOUT;
    if (i == 3) currentScreen = SCREEN_EXIT;
}

void updateMenuScreen()
{
    // Keyboard: up/down change the lit button (and wrap around).
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedMenuItem++;
        if (selectedMenuItem == MENU_ITEM_COUNT)
        {
            selectedMenuItem = 0;
        }
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedMenuItem--;
        if (selectedMenuItem < 0)
        {
            selectedMenuItem = MENU_ITEM_COUNT - 1;
        }
    }

    // Mouse: the button under the mouse lights up; a click opens it.
    for (int i = 0; i < MENU_ITEM_COUNT; i++)
    {
        if (isMouseOver(MENU_BUTTON_X, menuButtonY(i), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT))
        {
            selectedMenuItem = i;
        }
        if (isClicked(MENU_BUTTON_X, menuButtonY(i), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT))
        {
            openMenuItem(i);
            return;
        }
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        openMenuItem(selectedMenuItem);
    }
    else if (IsKeyPressed(KEY_ESCAPE))
    {
        currentScreen = SCREEN_EXIT;
    }
}

void drawMenuScreen()
{
    float centerX = WINDOW_WIDTH / 2.0f;

    drawGlitchTitle("ESCAPE THE", centerX, 120.0f, 64, appTime);
    drawGlitchTitle("MAINFRAME", centerX, 190.0f, 96, appTime + 0.9f);
    drawTextCentered("a labyrinth game in C++", centerX, 305.0f, 22, COLOR_TEXT_DIM);

    // The hacker and the exit on both sides of the title
    drawPlayer(centerX - 330.0f, 240.0f, 64.0f, DIR_RIGHT, appTime);
    drawExit(centerX + 330.0f, 240.0f, 64.0f, appTime);

    for (int i = 0; i < MENU_ITEM_COUNT; i++)
    {
        bool isSelected = (i == selectedMenuItem);
        drawButton(MENU_BUTTON_X, menuButtonY(i), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, menuLabels[i], isSelected, 28);
    }

    drawTextCentered("[UP/DOWN] choose    [ENTER] select    [F11] fullscreen",
                     centerX, WINDOW_HEIGHT - 50.0f, 18, COLOR_TEXT_DIM);
    drawUiText("team CYBERPUNKS", 20.0f, WINDOW_HEIGHT - 30.0f, 16, Fade(COLOR_NEON_RED, 0.7f));
}
