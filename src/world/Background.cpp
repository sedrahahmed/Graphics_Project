#include "Background.h"
#include "Draw.h"
#include "Colors.h"


static unsigned int backgroundTex = 0;

void initBackground() {
    backgroundTex = loadTexture(ASSET_DIR "background/background.png");
}
void drawBackground(float time) {
    drawSprite(backgroundTex,0,1,640,360,1280,720,0,WHITE); 
 }
