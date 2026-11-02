#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"


//Players
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


//Projectiles

void drawLanternOrb(float x, float y, float hitProgress) { drawCircle(x, y, 8, GHOST_LIGHT); }   // TODO
void drawCursedCard(float x, float y, float hitProgress) { drawRect(x, y, 12, 16, 0, WHITE); }   // TODO
void drawJuggleBall(float x, float y, float hitProgress) { drawCircle(x, y, 8, CURSE_RED); }     // TODO


//Collectibles

void drawToken(float x, float y, float time) { drawCircle(x, y, 10, TICKET_GOLD); }   // TODO


//Enemies

void drawMagician(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 45, 40, 90, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawClown(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 40, 50, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawJester(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}


//Health bar
// x, y = center of the bar. Placeholder: works already, make it prettier later.
void drawHealthBar(float x, float y, int current, int max) {
    float w = 50.0f, h = 6.0f;
    float frac = max > 0 ? (float)current / max : 0.0f;
    if (frac < 0.0f) frac = 0.0f; else if (frac > 1.0f) frac = 1.0f;
    glm::vec4 fillcolor = glm::mix(CURSE_RED, HEALTH_GREEN, frac);   // red -> green as health goes up
    drawRect(x, y, w+4, h+4, 0, TICKET_GOLD);
    drawRect(x, y, w, h, 0, BLACK);
    drawRect(x - w / 2 + (w * frac) / 2, y, w * frac, h, 0, fillcolor);
    float left = x-w/2;
    float segmentWidth = w/max;
    for(int i=1; i<max;i++){
        float dx=left + i * segmentWidth;
        drawLine(dx, y-h/2, dx, y+h/2, 1,BLACK);
    }
}

