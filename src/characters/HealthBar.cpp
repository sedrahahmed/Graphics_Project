// HealthBar.cpp. Owner: Person B
#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"

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
