// =============================================================================
//  game_screen.cpp  -  The game: keys, effects, side panel, victory window
// =============================================================================
#include <string>

#include "fonts.h"
#include "game.h"
#include "maze_renderer.h"
#include "particles.h"
#include "screens.h"
#include "theme.h"
#include "widgets.h"

using namespace std;

// Parts of the screen: the maze on the left, the side panel on the right
const float BOARD_X = 30, BOARD_Y = 30, BOARD_SIZE = 740;
const float PANEL_X = 800, PANEL_Y = 30, PANEL_WIDTH = 450, PANEL_HEIGHT = 740;

// Times in seconds
const float FIRST_STEP_DELAY = 0.18f;  // holding an arrow: wait before running
const float RUN_STEP_DELAY = 0.075f;   // holding an arrow: time between steps
const float HINT_TIME = 2.5f;
const float MESSAGE_TIME = 2.2f;
const float SHAKE_TIME = 0.18f;

bool hasGame = false;      // true while a maze is in memory
int chosenSize = 20;       // used by "new maze"

float playerDrawRow = 0;   // where the player is DRAWN; it slides smoothly
float playerDrawCol = 0;   // towards the real position (playerRow, playerCol)
int facing = DIR_RIGHT;    // where the player looks

int hintDirection = DIR_NONE;
float hintTimer = 0;       // seconds left to show the hint arrow

string message = "";       // the last event, e.g. "+1 DATA CHIP"
Color messageColor = WHITE;
float messageTimer = 0;    // seconds left to show the message

float moveTimer = 0;       // waiting time between steps while a key is held
float shakeTimer = 0;      // the board shakes after hitting a wall
float playTime = 0;        // seconds since the maze started
float victoryTimer = 0;    // seconds since winning (for the animations)

// =============================================================================
//  Small helpers
// =============================================================================

void showMessage(string text, Color color)
{
    message = text;
    messageColor = color;
    messageTimer = MESSAGE_TIME;
}

// The direction of an arrow / WASD key that was JUST pressed.
int getPressedDirection()
{
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return DIR_UP;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) return DIR_DOWN;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return DIR_LEFT;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return DIR_RIGHT;
    return DIR_NONE;
}

// The direction of an arrow / WASD key that is being HELD down.
int getHeldDirection()
{
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) return DIR_UP;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) return DIR_DOWN;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) return DIR_LEFT;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) return DIR_RIGHT;
    return DIR_NONE;
}

// "125" seconds -> "02:05"
string formatTime(float seconds)
{
    int minutes = (int)seconds / 60;
    int rest = (int)seconds % 60;
    return TextFormat("%02d:%02d", minutes, rest);
}

// =============================================================================
//  Starting and stopping
// =============================================================================

void startPlaying(int size)
{
    stopPlaying(); // free the old maze first (if there is one)

    int seed = GetRandomValue(0, 1000000000);
    startNewGame(size, seed);
    hasGame = true;
    chosenSize = size;

    playerDrawRow = 0;
    playerDrawCol = 0;
    facing = DIR_RIGHT;
    hintDirection = DIR_NONE;
    hintTimer = 0;
    moveTimer = 0;
    shakeTimer = 0;
    playTime = 0;
    victoryTimer = 0;
    showMessage("Find the EXIT!", COLOR_EXIT);

    clearParticles();
    currentScreen = SCREEN_PLAYING;
}

void stopPlaying()
{
    if (hasGame)
    {
        endGame();
        hasGame = false;
    }
}

// =============================================================================
//  Updating (keys and logic)
// =============================================================================

// Moves the player with the game logic and shows the right effect.
void handleMove(int direction)
{
    int result = movePlayer(direction);
    facing = direction;

    // Where the player is on the screen (the sparks start there)
    setBoardArea(BOARD_X, BOARD_Y, BOARD_SIZE, BOARD_SIZE);
    float x = tileCenterX((float)playerCol);
    float y = tileCenterY((float)playerRow);

    if (result == MOVE_WALL || result == MOVE_OUTSIDE)
    {
        shakeTimer = SHAKE_TIME;
        spawnBurst(x, y, COLOR_NEON_RED, 8, 110);
        showMessage("BZZT! Firewall.", COLOR_NEON_RED);
        return; // the player did not move, so the hint arrow stays
    }

    if (result == MOVE_COIN)
    {
        spawnBurst(x, y, COLOR_COIN, 30, 220);
        showMessage("+1 DATA CHIP  (+50)", COLOR_COIN);
    }
    else if (result == MOVE_DOOR)
    {
        spawnBurst(x, y, COLOR_DOOR, 40, 260);
        showMessage("BACKDOOR! Shortcut taken.", COLOR_DOOR);
    }
    else if (result == MOVE_EXIT)
    {
        spawnBurst(x, y, COLOR_EXIT, 90, 380);
        spawnConfetti(160);
        victoryTimer = 0;
    }

    hintTimer = 0; // after a step the old hint arrow disappears
}

// Keys while playing.
void updatePlaying(float deltaTime)
{
    playTime += deltaTime;

    if (IsKeyPressed(KEY_ESCAPE))
    {
        stopPlaying();
        currentScreen = SCREEN_MENU;
        return;
    }
    if (IsKeyPressed(KEY_R))
    {
        startPlaying(chosenSize);
        showMessage("New maze generated!", COLOR_PLAYER);
        return;
    }
    if (IsKeyPressed(KEY_H))
    {
        hintDirection = useHint();
        hintTimer = HINT_TIME;
        showMessage("Follow the arrow  (-30)", COLOR_PLAYER);
    }

    // Moving: one step for every press. If the key is held down, the player
    // starts running (one step every RUN_STEP_DELAY seconds).
    moveTimer -= deltaTime;
    int pressed = getPressedDirection();
    int held = getHeldDirection();

    if (pressed != DIR_NONE)
    {
        handleMove(pressed);
        moveTimer = FIRST_STEP_DELAY;
    }
    else if (held != DIR_NONE && moveTimer <= 0)
    {
        handleMove(held);
        moveTimer = RUN_STEP_DELAY;
    }
}

// Keys and confetti on the victory window.
void updateVictory(float deltaTime)
{
    victoryTimer += deltaTime;

    // A few new confetti pieces now and then
    if (GetRandomValue(0, 100) < 25)
    {
        spawnConfetti(2);
    }

    // Wait a moment, so a held arrow key does not close the window at once.
    if (victoryTimer < 0.6f)
    {
        return;
    }

    if (IsKeyPressed(KEY_ENTER) || isClicked(350, 590, 280, 60))
    {
        startPlaying(chosenSize);           // "PLAY AGAIN"
    }
    else if (IsKeyPressed(KEY_ESCAPE) || isClicked(650, 590, 280, 60))
    {
        stopPlaying();                      // "MENU"
        currentScreen = SCREEN_MENU;
    }
}

void updateGameScreen(float deltaTime)
{
    hintTimer -= deltaTime;
    messageTimer -= deltaTime;
    shakeTimer -= deltaTime;

    // The drawn player slides smoothly towards the real position:
    // every frame it covers a part of the distance that is left.
    float slide = deltaTime * 18;
    if (slide > 1)
    {
        slide = 1;
    }
    playerDrawRow += (playerRow - playerDrawRow) * slide;
    playerDrawCol += (playerCol - playerDrawCol) * slide;

    if (isWon)
    {
        updateVictory(deltaTime);
    }
    else
    {
        updatePlaying(deltaTime);
    }
}

// =============================================================================
//  Drawing
// =============================================================================

// One line in the side panel: name on the left, number on the right.
void drawStatRow(float y, string label, int value)
{
    string number = to_string(value);
    float right = PANEL_X + PANEL_WIDTH - 30;

    drawUiText(label, PANEL_X + 90, y + 12, 22, COLOR_TEXT_DIM);
    drawUiText(number, right - measureUiText(number, 34), y + 6, 34, COLOR_TEXT);
}

void drawSidePanel()
{
    float centerX = PANEL_X + PANEL_WIDTH / 2;
    float left = PANEL_X + 30;
    float right = PANEL_X + PANEL_WIDTH - 30;

    drawPanel(PANEL_X, PANEL_Y, PANEL_WIDTH, PANEL_HEIGHT, COLOR_NEON_RED);
    drawGlowText("ESCAPE THE MAINFRAME", centerX, 55, 28, COLOR_TEXT);

    string size = to_string(mazeSize);
    drawTextCentered("MAZE " + size + " x " + size + "      TIME " + formatTime(playTime),
                     centerX, 100, 20, COLOR_TEXT_DIM);

    // Statistics with small animated icons
    float iconX = PANEL_X + 50;
    drawPlayer(iconX, 172, 38, DIR_RIGHT, appTime);
    drawStatRow(150, "MOVES", moves);
    drawCoin(iconX, 234, 38, appTime);
    drawStatRow(212, "DATA CHIPS", coins);
    drawDoor(iconX, 296, 38, appTime);
    drawStatRow(274, "BACKDOORS USED", doorsUsed);
    drawHintArrow(iconX, 358, 38, DIR_RIGHT, appTime);
    drawStatRow(336, "HINTS", hintsUsed);

    // Signal bar: fills up as the player gets closer to the exit.
    int stepsLeft = getStepsToExit();
    float progress = 1.0f - (float)stepsLeft / optimalMoves;
    if (progress < 0)
    {
        progress = 0;
    }

    string stepsText = to_string(stepsLeft) + " steps to exit";
    drawUiText("SIGNAL", left, 420, 22, COLOR_TEXT_DIM);
    drawUiText(stepsText, right - measureUiText(stepsText, 22), 420, 22, COLOR_EXIT);

    float barWidth = right - left;
    DrawRectangleRec({ left, 455, barWidth, 18 }, COLOR_WALL);
    DrawRectangleRec({ left, 455, barWidth * progress, 18 }, COLOR_EXIT);
    DrawRectangleLinesEx({ left, 455, barWidth, 18 }, 2, COLOR_DARK_RED);

    // The score right now
    string scoreText = to_string(calculateScore());
    drawUiText("SCORE", left, 505, 22, COLOR_TEXT_DIM);
    drawUiText(scoreText, right - measureUiText(scoreText, 44), 495, 44, COLOR_COIN);

    // The last message, fading out at the end
    DrawRectangleLinesEx({ left, 565, barWidth, 60 }, 1, COLOR_DARK_RED);
    if (messageTimer > 0)
    {
        float alpha = messageTimer * 2;
        if (alpha > 1)
        {
            alpha = 1;
        }
        drawTextCentered(message, centerX, 584, 24, Fade(messageColor, alpha));
    }

    // Controls
    drawUiText("ARROWS / WASD  move", left, 655, 18, COLOR_TEXT_DIM);
    drawUiText("H  hint      R  new maze", left, 682, 18, COLOR_TEXT_DIM);
    drawUiText("ESC  menu", left, 709, 18, COLOR_TEXT_DIM);
}

void drawVictoryWindow()
{
    float centerX = WINDOW_WIDTH / 2.0f;

    // First the screen gets darker (for 1/3 second), then the window appears.
    float darkness = victoryTimer * 3;
    if (darkness < 1)
    {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.7f * darkness));
        return;
    }
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.7f));

    DrawRectangleRec({ 320, 120, 640, 560 }, COLOR_BACKGROUND);
    drawPanel(320, 120, 640, 560, COLOR_EXIT);
    drawGlitchTitle("ACCESS GRANTED", centerX, 150, 60, appTime);
    drawTextCentered("You escaped the mainframe!", centerX, 225, 24, COLOR_TEXT_DIM);

    // Statistics
    int extraMoves = moves - optimalMoves;
    drawTextCentered("Moves:  " + to_string(moves) + "   (shortest " + to_string(optimalMoves) +
                     ", extra " + to_string(extraMoves) + ")", centerX, 280, 24, COLOR_TEXT);
    drawTextCentered("Time:  " + formatTime(playTime) + "     Chips:  " + to_string(coins),
                     centerX, 316, 24, COLOR_TEXT);
    drawTextCentered("Backdoors:  " + to_string(doorsUsed) + "     Hints:  " + to_string(hintsUsed),
                     centerX, 352, 24, COLOR_TEXT);

    // The score counts up like in arcade games (1500 points per second).
    int score = calculateScore();
    int shownScore = (int)((victoryTimer - 0.35f) * 1500);
    if (shownScore < 0) shownScore = 0;
    if (shownScore > score) shownScore = score;
    drawGlowText(to_string(shownScore), centerX, 405, 72, COLOR_COIN);

    // When the counting is finished, show the rank.
    if (shownScore == score)
    {
        drawGlowText("RANK: " + getRankTitle(score), centerX, 500, 34, COLOR_EXIT);
    }

    drawButton(350, 590, 280, 60, "PLAY AGAIN [ENTER]", false, 22);
    drawButton(650, 590, 280, 60, "MENU [ESC]", false, 22);
}

void drawGameScreen()
{
    // After hitting a wall the board shakes a little (moves by up to 4 pixels).
    float shakeX = 0;
    float shakeY = 0;
    if (shakeTimer > 0)
    {
        shakeX = (float)GetRandomValue(-4, 4);
        shakeY = (float)GetRandomValue(-4, 4);
    }
    setBoardArea(BOARD_X + shakeX, BOARD_Y + shakeY, BOARD_SIZE, BOARD_SIZE);

    drawMazeBoard(appTime);

    // The hint arrow on the next tile
    if (hintTimer > 0 && hintDirection != DIR_NONE)
    {
        float hintCol = (float)(playerCol + colChange(hintDirection));
        float hintRow = (float)(playerRow + rowChange(hintDirection));
        drawHintArrow(tileCenterX(hintCol), tileCenterY(hintRow), tileSize, hintDirection, appTime);
    }

    drawPlayer(tileCenterX(playerDrawCol), tileCenterY(playerDrawRow), tileSize, facing, appTime);
    drawSidePanel();

    if (isWon)
    {
        drawVictoryWindow();
    }
}
