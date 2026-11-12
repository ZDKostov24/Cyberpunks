// =============================================================================
//  maze_renderer.cpp  -  Draws the maze, the player and all items
// =============================================================================
#include "maze_renderer.h"

#include <cmath>

#include "game.h"
#include "theme.h"
#include "widgets.h"

float boardLeft = 0.0f;
float boardTop = 0.0f;
float tileSize = 20.0f;

// Returns "value", but never less than "minimum".
// (Lines must stay visible even when the tiles are very small.)
float atLeast(float value, float minimum)
{
    if (value < minimum)
    {
        return minimum;
    }
    return value;
}

// A soft round glow: a circle that fades out towards its edge.
void drawGlow(float x, float y, float radius, Color color, float strength)
{
    DrawCircleGradient((int)x, (int)y, radius, Fade(color, strength), Fade(color, 0.0f));
}

// =============================================================================
//  Where the board is
// =============================================================================

void setBoardArea(float x, float y, float width, float height)
{
    // The tiles are square, so we use the shorter side of the area.
    float shortestSide = width;
    if (height < shortestSide)
    {
        shortestSide = height;
    }
    tileSize = floorf(shortestSide / mazeSize);

    // Put the board in the middle of the area.
    float boardSize = tileSize * mazeSize;
    boardLeft = x + (width - boardSize) / 2;
    boardTop = y + (height - boardSize) / 2;
}

float tileCenterX(float col)
{
    return boardLeft + (col + 0.5f) * tileSize;
}

float tileCenterY(float row)
{
    return boardTop + (row + 0.5f) * tileSize;
}

// =============================================================================
//  The board
// =============================================================================

// true while a secret door is "glitching". Most of the time a door looks
// exactly like a wall; every 2.4 seconds it glitches for a short moment.
bool isDoorGlitching(float time)
{
    return fmodf(time, 2.4f) < 0.35f;
}

// The glitch: the square is cut into 4 horizontal strips. Every strip jumps
// a little to the side and gets a random dark red shade - every frame anew.
void drawGlitchStrips(float left, float top, float size)
{
    Color shades[3] = { COLOR_WALL, COLOR_DARK_RED, { 90, 12, 24, 255 } };
    float stripHeight = size / 4;

    for (int strip = 0; strip < 4; strip++)
    {
        float jump = GetRandomValue(-2, 2) * size * 0.05f;
        Color color = shades[GetRandomValue(0, 2)];
        DrawRectangleRec({ left + jump, top + strip * stripHeight, size, stripHeight }, color);
    }
}

// A glowing red line from (x1, y1) to (x2, y2).
// brightness: 1 = normal, smaller = darker (used for the glitching doors).
void drawNeonLine(float x1, float y1, float x2, float y2, float thickness, float brightness)
{
    DrawLineEx({ x1, y1 }, { x2, y2 }, thickness * 3, Fade(COLOR_NEON_RED, 0.18f * brightness)); // glow
    DrawLineEx({ x1, y1 }, { x2, y2 }, thickness, Fade(COLOR_NEON_RED, brightness));
}

// true if (row, col) is a corridor. Secret doors do NOT count as corridor,
// so no neon line is drawn between a wall and a door - they look the same.
bool isCorridor(int row, int col)
{
    return isInsideMaze(row, col) && maze[row][col] != TILE_WALL && maze[row][col] != TILE_DOOR;
}

// A wall tile: dark inside, plus a neon line on every side that touches a
// corridor. Together these lines draw glowing outlines of the corridors.
// Secret doors are drawn the same way; when "glitching" is true, the whole
// square glitches and its lines get darker.
void drawWallTile(int row, int col, bool glitching)
{
    float left = boardLeft + col * tileSize;
    float top = boardTop + row * tileSize;
    float right = left + tileSize;
    float bottom = top + tileSize;
    float thickness = atLeast(tileSize * 0.09f, 1.5f);
    float brightness = 1.0f;

    if (glitching)
    {
        drawGlitchStrips(left, top, tileSize);
        brightness = 0.45f;
    }
    else
    {
        DrawRectangleRec({ left, top, tileSize, tileSize }, COLOR_WALL);
    }

    if (isCorridor(row - 1, col)) drawNeonLine(left, top, right, top, thickness, brightness);
    if (isCorridor(row + 1, col)) drawNeonLine(left, bottom, right, bottom, thickness, brightness);
    if (isCorridor(row, col - 1)) drawNeonLine(left, top, left, bottom, thickness, brightness);
    if (isCorridor(row, col + 1)) drawNeonLine(right, top, right, bottom, thickness, brightness);
}

// A 4-point star for the data chips: 4 long tips (up, right, down, left)
// and 4 corners bent deep inside between them. "rotation" turns it (degrees).
void drawFourPointStar(float x, float y, float radius, float rotation, Color color)
{
    float turn = rotation * DEG2RAD; // degrees -> radians

    for (int i = 0; i < 8; i++)
    {
        // Two neighbour points: a tip and an inner corner, going around.
        float angle1 = turn + i * PI / 4;
        float angle2 = turn + (i + 1) * PI / 4;
        float length1 = radius;
        float length2 = radius;
        if (i % 2 == 1) length1 = radius * 0.32f; // inner corner
        else            length2 = radius * 0.32f;

        float x1 = x + cosf(angle1) * length1;
        float y1 = y + sinf(angle1) * length1;
        float x2 = x + cosf(angle2) * length2;
        float y2 = y + sinf(angle2) * length2;

        DrawTriangle({ x, y }, { x2, y2 }, { x1, y1 }, color);
    }
}

void drawMazeBoard(float time)
{
    float boardSize = tileSize * mazeSize;

    // Floor and a neon frame around the board
    DrawRectangleRec({ boardLeft, boardTop, boardSize, boardSize }, COLOR_FLOOR);
    drawNeonFrame(boardLeft - 4, boardTop - 4, boardSize + 8, boardSize + 8, COLOR_NEON_RED, 2.0f);

    // First the floor and the walls...
    for (int row = 0; row < mazeSize; row++)
    {
        for (int col = 0; col < mazeSize; col++)
        {
            float x = tileCenterX((float)col);
            float y = tileCenterY((float)row);

            if (maze[row][col] == TILE_WALL)
            {
                drawWallTile(row, col, false);
            }
            else if (maze[row][col] == TILE_DOOR)
            {
                // A secret door looks like a wall, but glitches now and then.
                // Every door has its own "time", so they don't glitch together.
                float doorTime = time + (row * 7 + col) * 0.37f;
                drawWallTile(row, col, isDoorGlitching(doorTime));
            }
            else if (visited[row][col])
            {
                // The player's trail
                DrawCircleV({ x, y }, atLeast(tileSize * 0.08f, 1.5f), Fade(COLOR_PLAYER, 0.30f));
            }
            else
            {
                // A tiny dot on the floor
                DrawCircleV({ x, y }, 1.0f, Fade(COLOR_DARK_RED, 0.6f));
            }
        }
    }

    // ...then the items on top, so their glow is not covered by walls.
    for (int row = 0; row < mazeSize; row++)
    {
        for (int col = 0; col < mazeSize; col++)
        {
            float x = tileCenterX((float)col);
            float y = tileCenterY((float)row);

            // Every item gets its own "time", so they don't all move together.
            float itemTime = time + (row * 7 + col) * 0.37f;

            if (maze[row][col] == TILE_COIN) drawCoin(x, y, tileSize, itemTime);
            if (maze[row][col] == TILE_EXIT) drawExit(x, y, tileSize, time);
        }
    }
}

// =============================================================================
//  Single objects
// =============================================================================

void drawPlayer(float x, float y, float size, int facing, float time)
{
    // The body "breathes" (gets a little bigger and smaller).
    float radius = size * 0.34f * (1.0f + 0.05f * sinf(time * 6.0f));

    drawGlow(x, y, size * 1.1f, COLOR_PLAYER, 0.45f);
    DrawCircleV({ x, y }, radius, COLOR_PLAYER);        // bright ring
    DrawCircleV({ x, y }, radius * 0.74f, COLOR_VISOR); // dark visor

    // The eyes look in the direction of the last move.
    float lookX = colChange(facing) * radius * 0.22f;
    float lookY = rowChange(facing) * radius * 0.22f;

    float leftEyeX = x - radius * 0.32f + lookX;
    float rightEyeX = x + radius * 0.32f + lookX;
    float eyeY = y - radius * 0.05f + lookY;
    float eyeSize = atLeast(radius * 0.17f, 1.2f);

    bool isBlinking = fmod(time, 3.7f) < 0.12f; // blink every 3.7 seconds
    if (isBlinking)
    {
        DrawLineEx({ leftEyeX - eyeSize, eyeY }, { leftEyeX + eyeSize, eyeY }, 2.0f, WHITE);
        DrawLineEx({ rightEyeX - eyeSize, eyeY }, { rightEyeX + eyeSize, eyeY }, 2.0f, WHITE);
    }
    else
    {
        DrawCircleV({ leftEyeX, eyeY }, eyeSize, WHITE);
        DrawCircleV({ rightEyeX, eyeY }, eyeSize, WHITE);
    }
}

void drawCoin(float x, float y, float size, float time)
{
    // The chip is a golden 4-point star that floats up and down and spins.
    float floatingY = y + sinf(time * 3.0f) * size * 0.06f;
    float radius = size * 0.34f;
    float rotation = time * 120.0f;
    float edge = atLeast(size * 0.05f, 1.0f);

    drawGlow(x, floatingY, size * 0.6f, COLOR_COIN, 0.35f);
    drawFourPointStar(x, floatingY, radius + edge, rotation, WHITE);  // white edge
    drawFourPointStar(x, floatingY, radius, rotation, COLOR_COIN);
}

void drawDoor(float x, float y, float size, float time)
{
    // The icon of a secret door: a wall block that glitches now and then.
    float left = x - size / 2;
    float top = y - size / 2;
    float lineWidth = atLeast(size * 0.09f, 1.5f);

    if (isDoorGlitching(time))
    {
        drawGlitchStrips(left, top, size);
        DrawRectangleLinesEx({ left, top, size, size }, lineWidth, Fade(COLOR_NEON_RED, 0.45f));
    }
    else
    {
        DrawRectangleRec({ left, top, size, size }, COLOR_WALL);
        DrawRectangleLinesEx({ left, top, size, size }, lineWidth, COLOR_NEON_RED);
    }
}

void drawExit(float x, float y, float size, float time)
{
    float pulse = 0.5f + 0.5f * sinf(time * 4.0f);
    float lineWidth = atLeast(size * 0.05f, 1.0f);

    DrawRectangleRec({ x - size / 2, y - size / 2, size, size }, Fade(COLOR_EXIT, 0.15f));
    drawGlow(x, y, size * (1.0f + 0.4f * pulse), COLOR_EXIT, 0.5f);

    // Two squares spinning in opposite directions - a "portal"
    DrawPolyLinesEx({ x, y }, 4, size * 0.38f, time * 90.0f, lineWidth, COLOR_EXIT);
    DrawPolyLinesEx({ x, y }, 4, size * 0.24f, -time * 140.0f, lineWidth, WHITE);

    // The word EXIT only on big icons (menu, help)
    if (size >= 50.0f)
    {
        drawTextCentered("EXIT", x, y + size * 0.52f, (int)(size * 0.22f), COLOR_EXIT);
    }
}

void drawWallBlock(float x, float y, float size)
{
    DrawRectangleRec({ x - size / 2, y - size / 2, size, size }, COLOR_WALL);
    DrawRectangleLinesEx({ x - size / 2, y - size / 2, size, size }, atLeast(size * 0.09f, 1.5f), COLOR_NEON_RED);
}

void drawHintArrow(float x, float y, float size, int direction, float time)
{
    // A triangle pointing in the direction. 0 degrees = right, 90 = down ...
    float angle = 0.0f;
    if (direction == DIR_RIGHT) angle = 0.0f;
    if (direction == DIR_DOWN)  angle = 90.0f;
    if (direction == DIR_LEFT)  angle = 180.0f;
    if (direction == DIR_UP)    angle = 270.0f;
    if (direction == DIR_NONE)  return;

    float pulse = 0.5f + 0.5f * sinf(time * 10.0f);
    drawGlow(x, y, size * 0.8f, COLOR_PLAYER, 0.35f * pulse + 0.15f);
    DrawPoly({ x, y }, 3, size * 0.3f, angle, Fade(COLOR_PLAYER, 0.6f + 0.4f * pulse));
}
