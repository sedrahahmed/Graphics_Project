// HUD.cpp. Owner: Person D
#include "HUD.h"
#include "Draw.h"
#include "Colors.h"
#include "Text.h"
#include "Config.h"
#include "CharacterDraw.h"
#include <string>

void drawHUD(int coins, int wave) {
    drawTokenIcon(45, 660, 48, WHITE);   // the image has empty space around the coin, so draw it a bit bigger
    std::string coinText = std::to_string(coins);
    drawText(coinText, 72, 648, 0.7f, TICKET_GOLD);

    std::string waveText = "Wave " + std::to_string(wave) + " / 3";
    float waveScale = 0.7f;
    float waveX = ((SCREEN_W/2.0f)-(textWidth(waveText, waveScale)/2.0f));
    drawText(waveText, waveX, 648, waveScale, WHITE);

 }
