// Background.cpp. Owner: Person D
#include "Background.h"
#include "Draw.h"
#include "Colors.h"


static unsigned int backgroundTex = 0;

void initBackground() {
    backgroundTex = loadTexture(ASSET_DIR "background/background.png");
}
void drawBackground(float time) {
    drawRect(640,680,1280,80,0,glm::vec4(0.094f, 0.004f, 0.133f, 1.0f)); // sky
    drawSprite(backgroundTex,0,1,640,320,1280,640,0,WHITE); // background image
 }
