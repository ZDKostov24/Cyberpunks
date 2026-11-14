// =============================================================================
//  code_rain.cpp  -  The falling "code" in the background
//
//  Every column has a falling "head". Behind it there is a trail of symbols
//  that get darker and darker.
// =============================================================================
#include "code_rain.h"

#include "raylib.h"
#include "theme.h"

const int COLUMN_WIDTH = 26;   // pixels between columns
const int SYMBOL_HEIGHT = 24;  // pixels between symbols
const int TRAIL_LENGTH = 14;   // symbols in one trail
const int COLUMN_COUNT = WINDOW_WIDTH / COLUMN_WIDTH + 1;
const char SYMBOLS[] = "01{}<>/#$;=01";
const int SYMBOL_COUNT = 13;

float headY[COLUMN_COUNT];      // y position of the head of every column
float fallSpeed[COLUMN_COUNT];  // pixels per second
float rainTime = 0.0f;          // makes the symbols change

void initCodeRain()
{
    for (int column = 0; column < COLUMN_COUNT; column++)
    {
        headY[column] = (float)GetRandomValue(-WINDOW_HEIGHT, WINDOW_HEIGHT);
        fallSpeed[column] = (float)GetRandomValue(60, 220);
    }
}

void updateCodeRain(float deltaTime)
{
    rainTime += deltaTime;

    for (int column = 0; column < COLUMN_COUNT; column++)
    {
        headY[column] += fallSpeed[column] * deltaTime;

        // When the whole trail has left the screen, start again from the top.
        if (headY[column] - TRAIL_LENGTH * SYMBOL_HEIGHT > WINDOW_HEIGHT)
        {
            headY[column] = (float)GetRandomValue(-300, -20);
            fallSpeed[column] = (float)GetRandomValue(60, 220);
        }
    }
}

// A "random looking" symbol that changes 6 times per second.
char pickSymbol(int column, int row)
{
    int tick = (int)(rainTime * 6.0f);
    int index = (column * 7919 + row * 104729 + tick * 31) % SYMBOL_COUNT;
    if (index < 0)
    {
        index += SYMBOL_COUNT;
    }
    return SYMBOLS[index];
}

void drawCodeRain(float brightness)
{
    for (int column = 0; column < COLUMN_COUNT; column++)
    {
        int x = column * COLUMN_WIDTH + 4;
        int headRow = (int)headY[column] / SYMBOL_HEIGHT;

        for (int step = 0; step < TRAIL_LENGTH; step++)
        {
            int row = headRow - step;
            int y = row * SYMBOL_HEIGHT;

            // The head is white and bright, the end of the trail is almost invisible.
            float fade = 1.0f - (float)step / TRAIL_LENGTH;
            Color color = COLOR_NEON_RED;
            if (step == 0)
            {
                color = COLOR_TEXT;
            }

            char text[2] = { pickSymbol(column, row), '\0' };
            DrawText(text, x, y, 20, Fade(color, fade * 0.55f * brightness));
        }
    }
}
