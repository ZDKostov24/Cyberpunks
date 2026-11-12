// =============================================================================
//  maze_renderer.h  -  Draws the maze, the player and all items
// =============================================================================
#ifndef MAZE_RENDERER_H
#define MAZE_RENDERER_H

#include "raylib.h"

// Where the maze is on the screen. Set by setBoardArea().
extern float boardLeft;  // x of the left edge of the maze
extern float boardTop;   // y of the top edge of the maze
extern float tileSize;   // width = height of one tile in pixels

// Fits the maze into the rectangle (x, y, width, height) and centers it.
void setBoardArea(float x, float y, float width, float height);

// The pixel position of the middle of a tile.
// row and col can be decimals (used for the smooth movement of the player).
float tileCenterX(float col);
float tileCenterY(float row);

// Draws the whole board: floor, neon walls, trail, chips, doors and exit.
void drawMazeBoard(float time);

// Single objects at (x, y) with the given size.
// They are also used as icons in the help screen and the side panel.
void drawPlayer(float x, float y, float size, int facing, float time);
void drawCoin(float x, float y, float size, float time);
void drawDoor(float x, float y, float size, float time);
void drawExit(float x, float y, float size, float time);
void drawWallBlock(float x, float y, float size);
void drawHintArrow(float x, float y, float size, int direction, float time);

#endif // MAZE_RENDERER_H
