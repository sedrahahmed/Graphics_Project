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

static unsigned int pauseTexture = 0;

// loads the pause button picture (called once at startup)
void initHUD() {
    pauseTexture = loadTexture(ASSET_DIR "background/pause_button.png");
}

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
    // a bit darker normally, full color when the mouse is on it
    glm::vec4 tint = hover ? WHITE : glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
    drawSprite(pauseTexture, 0, 1, PAUSE_X, PAUSE_Y, 80, 75, 0, tint);
 }

 bool pauseIconClicked(){
    return mouseInRect(PAUSE_X, PAUSE_Y, 2*PAUSE_R, 2*PAUSE_R) && mouseClicked();
 }
