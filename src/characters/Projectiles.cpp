// Projectiles.cpp: how projectiles look. Owner: Person C
#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"

void drawLanternOrb(float x, float y, float hitProgress) { drawCircle(x, y, 8, GHOST_LIGHT); }   // TODO
void drawCursedCard(float x, float y, float hitProgress) { drawRect(x, y, 12, 16, 0, WHITE); }   // TODO
void drawJuggleBall(float x, float y, float hitProgress) { drawCircle(x, y, 8, CURSE_RED); }     // TODO
