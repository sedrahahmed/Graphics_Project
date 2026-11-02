// HUD.cpp. Owner: Person D
#include "HUD.h"
#include "Draw.h"
#include "Colors.h"
#include "Text.h"
#include "Config.h"
#include <string>

void drawHUD(int coins, int wave) {
    drawCircle(1150,660,18, TICKET_GOLD);
    drawCircle(1150,660,12, glm::vec4(0.75f, 0.55f, 0.10f, 1.0f));
    std::string coinText = std::to_string(coins);
    drawText(coinText, 1180, 648, 0.7f, TICKET_GOLD);

    std::string waveText = "Wave " + std::to_string(wave) + " / 3";
    float waveScale = 0.7f;
    float waveX = ((SCREEN_W/2.0f)-(textWidth(waveText, waveScale)/2.0f));
    drawText(waveText, waveX, 648, waveScale, WHITE);

 }
