// =============================================================================
//  widgets.h  -  Small drawing helpers: neon text, frames, panels, buttons
//
//  A rectangle is always given as: x, y (top-left corner), width, height.
// =============================================================================
#ifndef WIDGETS_H
#define WIDGETS_H

#include <string>

#include "raylib.h"

// Text whose middle is at centerX.
void drawTextCentered(std::string text, float centerX, float y, int fontSize, Color color);

// Centered text with a soft neon glow around it.
void drawGlowText(std::string text, float centerX, float y, int fontSize, Color color);

// Big title in the retro pixel font that "glitches" every few seconds.
void drawGlitchTitle(std::string text, float centerX, float y, int fontSize, float time);

// A rectangle outline with a neon glow.
void drawNeonFrame(float x, float y, float width, float height, Color color, float thickness);

// A dark panel with a neon border.
void drawPanel(float x, float y, float width, float height, Color borderColor);

// A button. "isSelected" makes it light up.
void drawButton(float x, float y, float width, float height, std::string label, bool isSelected, int fontSize);

// true when the mouse is over the rectangle.
bool isMouseOver(float x, float y, float width, float height);

// true when the rectangle was clicked with the left mouse button.
bool isClicked(float x, float y, float width, float height);

#endif // WIDGETS_H
