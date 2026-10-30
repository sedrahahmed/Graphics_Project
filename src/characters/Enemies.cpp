// Enemies.cpp: the three cursed performers. Owner: Person C
// Placeholder: gray boxes until the real drawings exist. x, y = FEET.
#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"

void drawMagician(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 45, 40, 90, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawClown(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 40, 50, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawJester(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
