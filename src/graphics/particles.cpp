// =============================================================================
//  particles.cpp  -  Small glowing sparks and confetti
//
//  Every spark is stored in the same place [i] of several arrays:
//  sparkX[i], sparkY[i], sparkSpeedX[i] ... all describe spark number i.
// =============================================================================
#include "particles.h"

#include <cmath>

#include "theme.h"

const int MAX_SPARKS = 3000;

bool  sparkAlive[MAX_SPARKS];   // false = this place is free
float sparkX[MAX_SPARKS];       // position
float sparkY[MAX_SPARKS];
float sparkSpeedX[MAX_SPARKS];  // pixels per second
float sparkSpeedY[MAX_SPARKS];
float sparkGravity[MAX_SPARKS]; // pulls the spark down
float sparkLife[MAX_SPARKS];    // seconds left
float sparkMaxLife[MAX_SPARKS]; // seconds at the start (for fading out)
float sparkSize[MAX_SPARKS];    // radius
Color sparkColor[MAX_SPARKS];

// A random decimal number between min and max.
float randomFloat(float min, float max)
{
    return min + (max - min) * GetRandomValue(0, 1000) / 1000.0f;
}

// Puts a new spark in the first free place.
void addSpark(float x, float y, float speedX, float speedY, float gravity,
              float life, float size, Color color)
{
    for (int i = 0; i < MAX_SPARKS; i++)
    {
        if (!sparkAlive[i])
        {
            sparkAlive[i] = true;
            sparkX[i] = x;
            sparkY[i] = y;
            sparkSpeedX[i] = speedX;
            sparkSpeedY[i] = speedY;
            sparkGravity[i] = gravity;
            sparkLife[i] = life;
            sparkMaxLife[i] = life;
            sparkSize[i] = size;
            sparkColor[i] = color;
            return;
        }
    }
}

void clearParticles()
{
    for (int i = 0; i < MAX_SPARKS; i++)
    {
        sparkAlive[i] = false;
    }
}

void spawnBurst(float x, float y, Color color, int amount, float speed)
{
    for (int i = 0; i < amount; i++)
    {
        // A random direction (angle) and a random speed.
        float angle = randomFloat(0.0f, 2.0f * PI);
        float sparkSpeed = randomFloat(speed * 0.3f, speed);
        float speedX = cosf(angle) * sparkSpeed;
        float speedY = sinf(angle) * sparkSpeed;

        addSpark(x, y, speedX, speedY, 120.0f, randomFloat(0.4f, 0.9f), randomFloat(1.5f, 3.5f), color);
    }
}

void spawnConfetti(int amount)
{
    Color colors[5] = { COLOR_NEON_RED, COLOR_PLAYER, COLOR_EXIT, COLOR_DOOR, COLOR_COIN };

    for (int i = 0; i < amount; i++)
    {
        float x = randomFloat(0.0f, WINDOW_WIDTH);
        float y = randomFloat(-60.0f, -5.0f);
        float speedX = randomFloat(-60.0f, 60.0f);
        float speedY = randomFloat(80.0f, 260.0f);
        Color color = colors[GetRandomValue(0, 4)];

        addSpark(x, y, speedX, speedY, 40.0f, randomFloat(2.5f, 4.5f), randomFloat(2.5f, 5.0f), color);
    }
}

void updateParticles(float deltaTime)
{
    for (int i = 0; i < MAX_SPARKS; i++)
    {
        if (!sparkAlive[i])
        {
            continue;
        }

        sparkLife[i] -= deltaTime;
        if (sparkLife[i] <= 0.0f)
        {
            sparkAlive[i] = false; // the spark is gone
            continue;
        }

        sparkSpeedY[i] += sparkGravity[i] * deltaTime;
        sparkX[i] += sparkSpeedX[i] * deltaTime;
        sparkY[i] += sparkSpeedY[i] * deltaTime;
    }
}

void drawParticles()
{
    BeginBlendMode(BLEND_ADDITIVE); // overlapping sparks glow brighter

    for (int i = 0; i < MAX_SPARKS; i++)
    {
        if (!sparkAlive[i])
        {
            continue;
        }

        float alpha = sparkLife[i] / sparkMaxLife[i]; // fades out at the end
        DrawCircleV({ sparkX[i], sparkY[i] }, sparkSize[i] * 2.2f, Fade(sparkColor[i], alpha * 0.25f)); // glow
        DrawCircleV({ sparkX[i], sparkY[i] }, sparkSize[i], Fade(sparkColor[i], alpha));
    }

    EndBlendMode();
}
