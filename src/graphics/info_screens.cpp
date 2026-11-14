// =============================================================================
//  info_screens.cpp  -  The "How to play" and "About" screens
// =============================================================================
#include <string>

#include "fonts.h"
#include "game.h"
#include "maze_renderer.h"
#include "screens.h"
#include "theme.h"
#include "widgets.h"

using namespace std;

// One line of the legend: a title and a short description next to an icon.
void drawLegendRow(float iconX, float iconY, string title, string text)
{
    drawUiText(title, iconX + 60, iconY - 20, 22, COLOR_TEXT);
    drawUiText(text, iconX + 60, iconY + 6, 18, COLOR_TEXT_DIM);
}

// One line of the controls: a key (cyan) and what it does.
void drawControlRow(float x, float y, string key, string action)
{
    drawUiText(key, x, y, 20, COLOR_PLAYER);
    drawUiText(action, x + 200, y, 20, COLOR_TEXT);
}

void drawBackHint()
{
    drawTextCentered("[ESC] or [ENTER] back to the menu", WINDOW_WIDTH / 2.0f,
                     WINDOW_HEIGHT - 50.0f, 18, COLOR_TEXT_DIM);
}

void updateInfoScreen()
{
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) ||
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        currentScreen = SCREEN_MENU;
    }
}

void drawHelpScreen()
{
    drawGlitchTitle("HOW TO PLAY", WINDOW_WIDTH / 2.0f, 50.0f, 56, appTime);

    // ---- Left panel: the mission, the rules and the controls ----
    drawPanel(60, 140, 560, 560, COLOR_NEON_RED);
    float x = 90;

    drawUiText("THE MISSION", x, 165, 26, COLOR_NEON_RED);
    drawUiText("You are a hacker trapped inside a mainframe.", x, 205, 20, COLOR_TEXT);
    drawUiText("Find the EXIT before the admin notices you!", x, 232, 20, COLOR_TEXT);

    drawUiText("RULES", x, 285, 26, COLOR_NEON_RED);
    drawUiText("- You start in the top-left corner (0, 0).", x, 325, 20, COLOR_TEXT);
    drawUiText("- The exit is in the bottom-right corner.", x, 352, 20, COLOR_TEXT);
    drawUiText("- Firewalls cannot be crossed...", x, 379, 20, COLOR_TEXT);
    drawUiText("  ...except secret BACKDOORS (shortcuts!)", x, 406, 20, COLOR_DOOR);
    drawUiText("- Every maze is new and random.", x, 433, 20, COLOR_TEXT);

    drawUiText("CONTROLS", x, 485, 26, COLOR_NEON_RED);
    drawControlRow(x, 525, "ARROWS / WASD", "move (hold to run)");
    drawControlRow(x, 552, "H", "hint arrow (-30 pts)");
    drawControlRow(x, 579, "R", "new maze");
    drawControlRow(x, 606, "ESC", "back to the menu");
    drawControlRow(x, 633, "F11", "fullscreen on / off");
    drawUiText("Score: 1000 +50/chip -5/extra move -30/hint", x, 670, 18, COLOR_COIN);

    // ---- Right panel: the legend with the real, animated objects ----
    drawPanel(660, 140, 560, 560, COLOR_PLAYER);
    drawUiText("LEGEND", 690, 165, 26, COLOR_PLAYER);

    float iconX = 720;
    float iconSize = 44;

    drawPlayer(iconX, 245, iconSize, DIR_RIGHT, appTime);
    drawLegendRow(iconX, 245, "YOU", "The hacker. Your eyes look where you go.");

    drawWallBlock(iconX, 327, iconSize);
    drawLegendRow(iconX, 327, "FIREWALL", "A wall. You can't pass.");

    drawDoor(iconX, 409, iconSize, appTime);
    drawLegendRow(iconX, 409, "BACKDOOR", "Looks like a wall, but glitches. A shortcut!");

    drawCoin(iconX, 491, iconSize, appTime);
    drawLegendRow(iconX, 491, "DATA CHIP", "+50 points.");

    drawExit(iconX, 573, iconSize, appTime);
    drawLegendRow(iconX, 573, "EXIT", "Reach it to escape!");

    drawBackHint();
}

void drawAboutScreen()
{
    float centerX = WINDOW_WIDTH / 2.0f;

    drawGlitchTitle("ABOUT", centerX, 70.0f, 56, appTime);
    drawPanel(centerX - 420, 170, 840, 470, COLOR_NEON_RED);

    drawGlowText("ESCAPE THE MAINFRAME", centerX, 210, 36, COLOR_TEXT);
    drawTextCentered("School project \"Labyrinth\" - made in C++ with raylib", centerX, 270, 22, COLOR_TEXT_DIM);
    drawGlowText("team CYBERPUNKS", centerX, 320, 30, COLOR_NEON_RED);

    drawTextCentered("Every maze is generated with a randomized depth-first search.", centerX, 400, 20, COLOR_TEXT);
    drawTextCentered("Shortest routes, hints and backdoors use breadth-first search.", centerX, 430, 20, COLOR_TEXT);
    drawTextCentered("The maze lives in dynamic arrays sized at runtime (10x10 to 50x50).", centerX, 460, 20, COLOR_TEXT);

    drawTextCentered("NO FACE. JUST CODE._", centerX, 560, 26, COLOR_NEON_RED);
    drawBackHint();
}
