// =============================================================================
//  fonts.h  -  The font for all normal text (buttons, panels, messages)
//
//  The big titles use raylib's built-in pixel font instead (see widgets.cpp).
// =============================================================================
#ifndef FONTS_H
#define FONTS_H

#include <string>

#include "raylib.h"

// Loads the "Chakra Petch" font from assets/fonts. Call after InitWindow().
// If the file is missing, the game simply uses the pixel font.
void loadGameFont();

// Frees the font. Call before CloseWindow().
void unloadGameFont();

// Draws text with the font at (x, y).
void drawUiText(std::string text, float x, float y, int fontSize, Color color);

// Width of the text in pixels.
float measureUiText(std::string text, int fontSize);

#endif // FONTS_H
