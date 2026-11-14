// =============================================================================
//  main.cpp  -  Start of the GRAPHICAL version (raylib)
//
//  The main loop runs 60 times per second. Every time it:
//    1. updates the current screen (reads keys, changes the variables)
//    2. draws the current screen
// =============================================================================
#include "raylib.h"
#include "rlgl.h"

#include "code_rain.h"
#include "fonts.h"
#include "particles.h"
#include "screens.h"
#include "theme.h"

int currentScreen = SCREEN_MENU;
float appTime = 0.0f;

void updateApp(float deltaTime)
{
    // These run on every screen:
    appTime += deltaTime;        // the time for all animations
    updateCodeRain(deltaTime);   // the falling code in the background
    updateParticles(deltaTime);  // sparks and confetti

    // "else if" is important here: only ONE screen is updated per frame.
    // Otherwise, when the menu opens "How to play", the same click/Enter
    // would be seen by the new screen too and it would close at once.
    if (currentScreen == SCREEN_MENU)             updateMenuScreen();
    else if (currentScreen == SCREEN_SIZE_SELECT) updateSizeScreen();
    else if (currentScreen == SCREEN_HELP)        updateInfoScreen();
    else if (currentScreen == SCREEN_ABOUT)       updateInfoScreen();
    else if (currentScreen == SCREEN_PLAYING)     updateGameScreen(deltaTime);
}

void drawApp()
{
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_BACKGROUND);

    // The code rain is darker during the game, so the maze is easy to see.
    if (currentScreen == SCREEN_PLAYING)
    {
        drawCodeRain(0.2f);
    }
    else
    {
        drawCodeRain(1.0f);
    }

    if (currentScreen == SCREEN_MENU)             drawMenuScreen();
    else if (currentScreen == SCREEN_SIZE_SELECT) drawSizeScreen();
    else if (currentScreen == SCREEN_HELP)        drawHelpScreen();
    else if (currentScreen == SCREEN_ABOUT)       drawAboutScreen();
    else if (currentScreen == SCREEN_PLAYING)     drawGameScreen();

    drawParticles();

    // Thin dark lines over everything - looks like an old monitor.
    for (int y = 0; y < WINDOW_HEIGHT; y += 4)
    {
        DrawRectangle(0, y, WINDOW_WIDTH, 1, Fade(BLACK, 0.18f));
    }
}

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Escape the Mainframe");
    SetExitKey(KEY_NULL); // ESC is used by the game (back to the menu), not to close it
    SetTargetFPS(60);
    ToggleBorderlessWindowed(); // start in fullscreen

    // The pixel font of the titles stays sharp when it is scaled up.
    SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);
    loadGameFont();
    initCodeRain();

    while (!WindowShouldClose() && currentScreen != SCREEN_EXIT)
    {
        // F11 switches between fullscreen and a window.
        if (IsKeyPressed(KEY_F11))
        {
            ToggleBorderlessWindowed();
        }

        // ---- Scaling ----
        // The game is drawn for a 1280 x 800 screen. On another screen size we
        // zoom in or out so it fits, and center it (black bars on the sides).
        float screenWidth = (float)GetScreenWidth();
        float screenHeight = (float)GetScreenHeight();
        float scale = screenWidth / WINDOW_WIDTH;
        if (screenHeight / WINDOW_HEIGHT < scale)
        {
            scale = screenHeight / WINDOW_HEIGHT;
        }
        float offsetX = (screenWidth - WINDOW_WIDTH * scale) / 2;
        float offsetY = (screenHeight - WINDOW_HEIGHT * scale) / 2;

        // The mouse must be scaled the same way, so the buttons still work.
        SetMouseOffset((int)-offsetX, (int)-offsetY);
        SetMouseScale(1 / scale, 1 / scale);

        // ---- Update ----
        float deltaTime = GetFrameTime(); // seconds since the last frame
        if (deltaTime > 0.05f)
        {
            deltaTime = 0.05f; // after a lag, don't let the animations jump
        }
        updateApp(deltaTime);

        // ---- Draw ----
        BeginDrawing();
        ClearBackground(BLACK);

        rlPushMatrix();                      // remember the normal drawing settings
        rlTranslatef(offsetX, offsetY, 0);  // move everything to the middle
        rlScalef(scale, scale, 1);           // make everything bigger or smaller
        drawApp();
        rlPopMatrix();                       // back to the normal settings

        // Black bars on the sides cover anything drawn outside the game area.
        DrawRectangle(0, 0, (int)offsetX, (int)screenHeight, BLACK);
        DrawRectangle((int)(screenWidth - offsetX), 0, (int)offsetX + 1, (int)screenHeight, BLACK);
        DrawRectangle(0, 0, (int)screenWidth, (int)offsetY, BLACK);
        DrawRectangle(0, (int)(screenHeight - offsetY), (int)screenWidth, (int)offsetY + 1, BLACK);

        EndDrawing();
    }

    stopPlaying(); // free the maze, if a game is still open
    unloadGameFont();
    CloseWindow();
    return 0;
}
