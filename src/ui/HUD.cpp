// HUD.cpp. Owner: Person D
#include "HUD.h"
#include "Draw.h"
#include "Colors.h"
#include "Text.h"
#include "Config.h"
#include "CharacterDraw.h"
#include <string>
#include "Input.h"


static const float PAUSE_X = SCREEN_W-40;
static const float PAUSE_Y = SCREEN_H-40;
static const float PAUSE_R = 22;

void drawHUD(int coins, int wave) {
    drawTokenIcon(45, 660, 48, WHITE);
    std::string coinText = std::to_string(coins);
    drawText(coinText, 72, 648, 0.7f, TICKET_GOLD);

    std::string waveText = "Wave " + std::to_string(wave) + " / 3";
    float waveScale = 0.7f;
    float waveX = ((SCREEN_W/2.0f)-(textWidth(waveText, waveScale)/2.0f)); //centered
    drawText(waveText, waveX, 648, waveScale, WHITE);
    drawPauseIcon();

 }

 void drawPauseIcon(){
    bool hover = mouseInRect(PAUSE_X, PAUSE_Y, 2*PAUSE_R, 2*PAUSE_R);
    drawCircle(PAUSE_X,PAUSE_Y,PAUSE_R+2,TICKET_GOLD);
    drawCircle(PAUSE_X,PAUSE_Y,PAUSE_R,hover ? glm::mix(CARD_PURPLE, WHITE, 0.3f) : CARD_PURPLE);
    drawRect(PAUSE_X-6,PAUSE_Y,6,20,0,TICKET_GOLD);
    drawRect(PAUSE_X+6,PAUSE_Y,6,20,0,TICKET_GOLD);
 }

 bool pauseIconClicked(){
    return mouseInRect(PAUSE_X, PAUSE_Y, 2*PAUSE_R, 2*PAUSE_R) && mouseClicked();
 }
