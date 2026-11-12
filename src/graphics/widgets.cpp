// =============================================================================
//  widgets.cpp  -  Small drawing helpers: neon text, frames, panels, buttons
// =============================================================================
#include "widgets.h"

#include <cmath>

#include "fonts.h"
#include "theme.h"

using namespace std;

// Draws centered text. If pixelFont is true, the retro raylib font is used
// (only for the titles); otherwise the normal game font.
void drawCenteredInFont(string text, float centerX, float y, int fontSize, Color color, bool pixelFont)
{
    if (pixelFont)
    {
        int width = MeasureText(text.c_str(), fontSize);
        DrawText(text.c_str(), (int)(centerX - width / 2.0f), (int)y, fontSize, color);
    }
    else
    {
        float width = measureUiText(text, fontSize);
        drawUiText(text, centerX - width / 2.0f, y, fontSize, color);
    }
}

// The glow: several faded copies of the text around it, then the text itself.
void drawGlowInFont(string text, float centerX, float y, int fontSize, Color color, bool pixelFont)
{
    Color glowColor = Fade(color, 0.12f);
    int spread = fontSize / 14 + 1;

    for (int offset = spread * 2; offset > 0; offset -= spread)
    {
        drawCenteredInFont(text, centerX - offset, y, fontSize, glowColor, pixelFont);
        drawCenteredInFont(text, centerX + offset, y, fontSize, glowColor, pixelFont);
        drawCenteredInFont(text, centerX, y - offset, fontSize, glowColor, pixelFont);
        drawCenteredInFont(text, centerX, y + offset, fontSize, glowColor, pixelFont);
    }
    drawCenteredInFont(text, centerX, y, fontSize, color, pixelFont);
}

void drawTextCentered(string text, float centerX, float y, int fontSize, Color color)
{
    drawCenteredInFont(text, centerX, y, fontSize, color, false);
}

void drawGlowText(string text, float centerX, float y, int fontSize, Color color)
{
    drawGlowInFont(text, centerX, y, fontSize, color, false);
}

void drawGlitchTitle(string text, float centerX, float y, int fontSize, float time)
{
    // A cyan and a red copy are slightly shifted. Every 3 seconds, for a moment,
    // they jump randomly - that is the "glitch".
    float jump = 0.0f;
    if (fmod(time, 3.0f) < 0.18f)
    {
        jump = (float)GetRandomValue(-8, 8);
    }

    drawCenteredInFont(text, centerX - 4 + jump, y + 2, fontSize, Fade(COLOR_PLAYER, 0.55f), true);
    drawCenteredInFont(text, centerX + 4 - jump, y - 2, fontSize, Fade(COLOR_NEON_RED, 0.85f), true);
    drawGlowInFont(text, centerX, y, fontSize, COLOR_TEXT, true);
}

void drawNeonFrame(float x, float y, float width, float height, Color color, float thickness)
{
    // 3 bigger, faded outlines make the glow.
    for (int layer = 3; layer >= 1; layer--)
    {
        float grow = layer * 3.0f;
        float alpha = 0.08f * (4 - layer);
        DrawRectangleLinesEx({ x - grow, y - grow, width + grow * 2, height + grow * 2 }, thickness, Fade(color, alpha));
    }
    DrawRectangleLinesEx({ x, y, width, height }, thickness, color);
}

void drawPanel(float x, float y, float width, float height, Color borderColor)
{
    DrawRectangleRec({ x, y, width, height }, COLOR_PANEL);
    drawNeonFrame(x, y, width, height, borderColor, 2.0f);
}

void drawButton(float x, float y, float width, float height, string label, bool isSelected, int fontSize)
{
    bool isLit = isSelected || isMouseOver(x, y, width, height);

    if (isLit)
    {
        DrawRectangleRec({ x, y, width, height }, Fade(COLOR_NEON_RED, 0.22f));
        drawNeonFrame(x, y, width, height, COLOR_NEON_RED, 2.0f);
        drawTextCentered(label, x + width / 2, y + (height - fontSize) / 2, fontSize, COLOR_TEXT);
    }
    else
    {
        DrawRectangleRec({ x, y, width, height }, Fade(COLOR_BACKGROUND, 0.85f));
        DrawRectangleLinesEx({ x, y, width, height }, 2.0f, COLOR_DARK_RED);
        drawTextCentered(label, x + width / 2, y + (height - fontSize) / 2, fontSize, COLOR_TEXT_DIM);
    }
}

bool isMouseOver(float x, float y, float width, float height)
{
    float mouseX = (float)GetMouseX();
    float mouseY = (float)GetMouseY();
    return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
}

bool isClicked(float x, float y, float width, float height)
{
    return isMouseOver(x, y, width, height) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}
