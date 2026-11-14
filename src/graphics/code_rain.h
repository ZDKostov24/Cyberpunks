// =============================================================================
//  code_rain.h  -  The falling "code" in the background
// =============================================================================
#ifndef CODE_RAIN_H
#define CODE_RAIN_H

// Gives every column a random start position and speed.
void initCodeRain();

// Moves the columns down. Must be called every frame!
void updateCodeRain(float deltaTime);

// brightness: 0 = invisible, 1 = full.
void drawCodeRain(float brightness);

#endif // CODE_RAIN_H
