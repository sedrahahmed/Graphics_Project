// Players.cpp: the three guards. Owner: Person B
// Placeholder: gray boxes until the real drawings exist. x, y = FEET.
#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"

void drawMirrorGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawLanternGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 35, 40, 70, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawDefenseGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 50, 60, 100, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));  // TODO
}

void drawGuard(float x, float y, PlayerType type, float time, AnimState state, float stateTime, float healthFrac) {
    switch (type) {
        case MIRROR_GUARD:   drawMirrorGuard(x, y, time, state, stateTime, healthFrac); break;
        case LANTERN_GUARD:  drawLanternGuard(x, y, time, state, stateTime, healthFrac); break;
        case DEFENSE_GUARD:  drawDefenseGuard(x, y, time, state, stateTime, healthFrac); break;
    }
}