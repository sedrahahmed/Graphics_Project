#pragma once
#include "Entities.h"

// x, y      = position of the character's FEET, centered horizontally
// time      = seconds since the game started (for idle wiggles, blinking)
// state     = WALK / ATTACK / JUMP / DUCK / IDLE
// stateTime = seconds spent in the current state (for attack swings etc.)
// healthFrac= health / maxHealth  (1.0 = full, drives the red tint)

// ---- Guards ----
void drawMirrorGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac);
void drawLanternGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac);
void drawSentinelGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac);

// ---- Cursed performers ----
void drawMagician(float x, float y, float time, AnimState state, float stateTime, float healthFrac);
void drawClown(float x, float y, float time, AnimState state, float stateTime, float healthFrac);
void drawJester(float x, float y, float time, AnimState state, float stateTime, float healthFrac);

// ---- Projectiles (hitProgress: 0 = flying, 0..1 = impact animation) ----
void drawLanternOrb(float x, float y, float hitProgress);
void drawCursedCard(float x, float y, float hitProgress);   // Magician, from above
void drawJuggleBall(float x, float y, float hitProgress);   // Clown, along the ground

// ---- Misc ----
void drawToken(float x, float y, float time);
void drawHealthBar(float x, float y, int current, int max);