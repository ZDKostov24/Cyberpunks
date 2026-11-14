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
bool themeDropdownOpen = false;

// All menu buttons have the same size and are under each other.
const float MENU_BUTTON_X = 460.0f;
const float MENU_BUTTON_WIDTH = 360.0f;
const float MENU_BUTTON_HEIGHT = 58.0f;

// The y position of menu button number i (0 = top button).
float menuButtonY(int i)
{
    return 390.0f + i * 74.0f;
}

void cycleTheme(int direction)
{
    int next = currentTheme + direction;
    if (next < 0) next = THEME_COUNT - 1;
    if (next >= THEME_COUNT) next = 0;
    setTheme(next);
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

    if (isClicked(1030, 24, 220, 52))
    {
        themeDropdownOpen = !themeDropdownOpen;
        return;
    }

    if (themeDropdownOpen)
    {
        for (int i = 0; i < THEME_COUNT; ++i)
        {
            if (isClicked(990, 105 + i * 38, 260, 34))
            {
                setTheme(i);
                return;
            }
        }
        for (int i = 0; i < PLAYER_COLOR_COUNT; ++i)
        {
            if (isClicked(990, 245 + i * 38, 260, 34))
            {
                setPlayerColor(i);
                return;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            themeDropdownOpen = false;
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
    drawButton(1030, 24, 220, 52, string("THEME: ") + getThemeName(),
               isMouseOver(1030, 24, 220, 52), 22);
    if (themeDropdownOpen)
    {
        DrawRectangleRec({ 980, 82, 280, 284 }, COLOR_PANEL);
        drawNeonFrame(980, 82, 280, 284, COLOR_NEON_RED, 2.0f);
        drawUiText("THEME", 998, 88, 18, COLOR_TEXT_DIM);
        const char* themeNames[THEME_COUNT] = { "RED", "BLUE", "YELLOW" };
        for (int i = 0; i < THEME_COUNT; ++i)
        {
            DrawRectangleRec({ 990, (float)(105 + i * 38), 260, 34 },
                             currentTheme == i ? Fade(COLOR_NEON_RED, 0.30f) : COLOR_BACKGROUND);
            drawUiText(themeNames[i], 1002, (float)(112 + i * 38), 18,
                       currentTheme == i ? COLOR_TEXT : COLOR_TEXT_DIM);
        }
        drawUiText("PLAYER COLOR", 998, 220, 18, COLOR_TEXT_DIM);
        const char* playerNames[PLAYER_COLOR_COUNT] = { "CYAN", "PINK", "GREEN" };
        Color playerColors[PLAYER_COLOR_COUNT] = {
            { 0, 229, 255, 255 }, { 255, 90, 210, 255 }, { 100, 255, 130, 255 }
        };
        for (int i = 0; i < PLAYER_COLOR_COUNT; ++i)
        {
            DrawRectangleRec({ 990, (float)(245 + i * 38), 260, 34 },
                             currentPlayerColor == i ? Fade(COLOR_NEON_RED, 0.30f) : COLOR_BACKGROUND);
            DrawCircle(1007, 262 + i * 38, 8, playerColors[i]);
            drawUiText(playerNames[i], 1024, (float)(252 + i * 38), 18,
                       currentPlayerColor == i ? COLOR_TEXT : COLOR_TEXT_DIM);
        }
    }

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
