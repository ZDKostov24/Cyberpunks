// =============================================================================
//  size_screen.cpp  -  Choosing the maze size (with input validation)
// =============================================================================
#include <string>

#include "game.h"
#include "screens.h"
#include "theme.h"
#include "widgets.h"

using namespace std;

const int DEFAULT_SIZE = 20;

string sizeText = "20";  // what the player typed
string sizeError = "";   // the error message ("" = no error)

// Ready-made difficulty buttons
const int PRESET_COUNT = 4;
string presetLabels[PRESET_COUNT] = { "EASY 10", "NORMAL 20", "HARD 35", "INSANE 50" };
int presetSizes[PRESET_COUNT] = { 10, 20, 35, 50 };

// The x position of preset button number i.
float presetButtonX(int i)
{
    return 210.0f + i * 220.0f;
}

// Adds "amount" to the typed size, but stays inside 10..50.
void changeSize(int amount)
{
    int size = DEFAULT_SIZE;
    if (isWholeNumber(sizeText))
    {
        size = textToNumber(sizeText);
    }

    size = size + amount;
    if (size < MIN_MAZE_SIZE) size = MIN_MAZE_SIZE;
    if (size > MAX_MAZE_SIZE) size = MAX_MAZE_SIZE;

    sizeText = to_string(size);
    sizeError = "";
}

// Checks the typed text. Starts the game, or shows what is wrong.
void tryToStart()
{
    if (!isWholeNumber(sizeText))
    {
        sizeError = "Please type a number.";
    }
    else if (!isValidMazeSize(textToNumber(sizeText)))
    {
        sizeError = "The size must be from 10 to 50.";
    }
    else
    {
        startPlaying(textToNumber(sizeText));
    }
}

// Reads the typed keys. Only digits are accepted, at most 2 of them.
void readTypedKeys()
{
    int key = GetCharPressed(); // 0 = nothing more was typed
    while (key > 0)
    {
        if (key < '0' || key > '9')
        {
            sizeError = "Only digits are allowed!";
        }
        else if (sizeText.size() >= 2)
        {
            sizeError = "Maximum 2 digits.";
        }
        else
        {
            sizeText = sizeText + (char)key;
            sizeError = "";
        }
        key = GetCharPressed();
    }

    // Backspace deletes the last digit (also when it is held down).
    bool backspace = IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE);
    if (backspace && sizeText.size() > 0)
    {
        sizeText.pop_back();
        sizeError = "";
    }
}

void openSizeScreen()
{
    sizeText = to_string(DEFAULT_SIZE);
    sizeError = "";
    currentScreen = SCREEN_SIZE_SELECT;
}

void updateSizeScreen()
{
    readTypedKeys();

    // The "+" and "-" buttons and the arrow keys
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_RIGHT) || isClicked(770, 270, 70, 70))
    {
        changeSize(1);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || isClicked(440, 270, 70, 70))
    {
        changeSize(-1);
    }

    for (int i = 0; i < PRESET_COUNT; i++)
    {
        if (isClicked(presetButtonX(i), 440, 200, 54))
        {
            sizeText = to_string(presetSizes[i]);
            sizeError = "";
        }
    }

    if (IsKeyPressed(KEY_ENTER) || isClicked(460, 590, 360, 64))
    {
        tryToStart();
    }
    else if (IsKeyPressed(KEY_ESCAPE))
    {
        currentScreen = SCREEN_MENU;
    }
}

void drawSizeScreen()
{
    float centerX = WINDOW_WIDTH / 2.0f;
    bool hasError = (sizeError != "");

    drawGlitchTitle("CHOOSE MAZE SIZE", centerX, 90.0f, 56, appTime);
    drawTextCentered("Type a number from 10 to 50, use the arrows, or pick a preset.",
                     centerX, 175.0f, 22, COLOR_TEXT_DIM);

    // The input box: red when there is an error, cyan otherwise
    DrawRectangleRec({ 530, 250, 220, 110 }, COLOR_PANEL);
    if (hasError)
    {
        drawNeonFrame(530, 250, 220, 110, COLOR_NEON_RED, 3.0f);
    }
    else
    {
        drawNeonFrame(530, 250, 220, 110, COLOR_PLAYER, 3.0f);
    }

    // The typed number with a blinking "_" cursor (2 times per second)
    string shownText = sizeText + " ";
    if ((int)(appTime * 2.0f) % 2 == 0)
    {
        shownText = sizeText + "_";
    }
    drawTextCentered(shownText, centerX + 10.0f, 272.0f, 70, COLOR_TEXT);

    drawButton(440, 270, 70, 70, "-", false, 64);
    drawButton(770, 270, 70, 70, "+", false, 56);

    // "20 x 20 tiles" under the box
    if (isWholeNumber(sizeText) && isValidMazeSize(textToNumber(sizeText)))
    {
        drawTextCentered(sizeText + " x " + sizeText + " tiles", centerX, 375.0f, 22, COLOR_TEXT_DIM);
    }

    for (int i = 0; i < PRESET_COUNT; i++)
    {
        bool isChosen = (sizeText == to_string(presetSizes[i]));
        drawButton(presetButtonX(i), 440, 200, 54, presetLabels[i], isChosen, 22);
    }

    if (hasError)
    {
        drawGlowText(sizeError, centerX, 525.0f, 26, COLOR_NEON_RED);
    }

    drawButton(460, 590, 360, 64, "START  >>", false, 30);
    drawTextCentered("[ENTER] start    [ESC] back", centerX, WINDOW_HEIGHT - 50.0f, 18, COLOR_TEXT_DIM);
}
