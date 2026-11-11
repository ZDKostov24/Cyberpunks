// =============================================================================
//  fonts.cpp  -  The font for all normal text (buttons, panels, messages)
// =============================================================================
#include "fonts.h"

using namespace std;

Font uiFont;            // "Font" is a type from raylib
bool hasUiFont = false; // false = the file was not found, use the pixel font

void loadGameFont()
{
    // CMake copies the "assets" folder next to the .exe file.
    string path = string(GetApplicationDirectory()) + "assets/fonts/ChakraPetch-SemiBold.ttf";
    if (!FileExists(path.c_str()))
    {
        return;
    }

    // Load the font in a big size; smaller text is made by scaling it down.
    // Mipmaps + trilinear filter keep small text smooth.
    uiFont = LoadFontEx(path.c_str(), 96, nullptr, 0);
    GenTextureMipmaps(&uiFont.texture);
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_TRILINEAR);
    hasUiFont = true;
}

void unloadGameFont()
{
    if (hasUiFont)
    {
        UnloadFont(uiFont);
        hasUiFont = false;
    }
}

void drawUiText(string text, float x, float y, int fontSize, Color color)
{
    if (!hasUiFont)
    {
        DrawText(text.c_str(), (int)x, (int)y, fontSize, color);
        return;
    }
    float letterSpacing = fontSize * 0.04f;
    DrawTextEx(uiFont, text.c_str(), { x, y }, (float)fontSize, letterSpacing, color);
}

float measureUiText(string text, int fontSize)
{
    if (!hasUiFont)
    {
        return (float)MeasureText(text.c_str(), fontSize);
    }
    float letterSpacing = fontSize * 0.04f;
    return MeasureTextEx(uiFont, text.c_str(), (float)fontSize, letterSpacing).x;
}
