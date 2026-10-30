// HealthBar.cpp. Owner: Person B
#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"

// x, y = center of the bar. Placeholder: works already, make it prettier later.
void drawHealthBar(float x, float y, int current, int max) {
    float w = 50.0f, h = 6.0f;
    float frac = max > 0 ? (float)current / max : 0.0f;
    drawRect(x, y, w, h, 0, BLACK);
    drawRect(x - w / 2 + (w * frac) / 2, y, w * frac, h, 0, HEALTH_GREEN);
}
