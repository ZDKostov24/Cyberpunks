// =============================================================================
//  particles.h  -  Small glowing sparks and confetti
// =============================================================================
#ifndef PARTICLES_H
#define PARTICLES_H

#include "raylib.h"

// Removes all sparks.
void clearParticles();

// A round explosion of "amount" sparks around (x, y).
void spawnBurst(float x, float y, Color color, int amount, float speed);

// Colorful confetti falling from the top of the screen.
void spawnConfetti(int amount);

// Moves the sparks and removes the old ones.
void updateParticles(float deltaTime);

void drawParticles();

#endif // PARTICLES_H
