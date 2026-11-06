#pragma once
#include "Entities.h"
#include <glm/glm.hpp>

void drawMeleeSeif(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX = false);
void drawRangedLama(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX = false);
void drawDefenseJana(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX = false);
void drawGuard(float x, float y, PlayerType type, float time, AnimState state, float stateTime, float healthFrac, bool flipX = false);
void drawEnemy(float x, float y, EnemyType type, float time, AnimState state, float stateTime, float healthFrac);
void drawMagician(float x, float y, float time, AnimState state, float stateTime, float healthFrac);
void drawClown(float x, float y, float time, AnimState state, float stateTime, float healthFrac);
void drawJester(float x, float y, float time, AnimState state, float stateTime, float healthFrac);
void drawDart(float x, float y, float time, float hitProgress);
void drawSpark(float x, float y, float time, float hitProgress);
void drawJuggleBall(float x, float y, float time, float hitProgress);
void drawToken(float x, float y);
void drawTokenIcon(float x, float y, float size, glm::vec4 tint);   // x, y = center (for the HUD and card prices)
void drawHealthBar(float x, float y, int current, int max);
void initCharacterTexture();
void drawGuardIcon(PlayerType type, float x, float y, float w,float h, glm::vec4 tint);