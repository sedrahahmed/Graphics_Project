// Environment.cpp. Owner: Person D
#include "Environment.h"
#include "Draw.h"
#include "Colors.h"

static unsigned int backgroundTex1 = 0;
static unsigned int backgroundTex2 = 0;
static unsigned int backgroundTex3 = 0;
static unsigned int backgroundTex4 = 0;
static unsigned int backgroundTex5 = 0;

void initEnvironment() {
    backgroundTex1 = loadTexture(ASSET_DIR "background/balloon_booth.png");
    backgroundTex2 = loadTexture(ASSET_DIR "background/circus_tent.png");
    backgroundTex3 = loadTexture(ASSET_DIR "background/Moon.png");
    backgroundTex4 = loadTexture(ASSET_DIR "background/popcorn_cart_with_flag.png");
    backgroundTex5 = loadTexture(ASSET_DIR "background/carousel.png");
}
void drawEnvironment(float time) { 
    drawSprite(backgroundTex1,0,1,271,489,130,157,0,WHITE); // moon
    drawSprite(backgroundTex2,0,1,676,492,177,163,0,WHITE); // circus tent
    drawSprite(backgroundTex5,0,1,896,488,141,154,0,WHITE); // carousel
    drawSprite(backgroundTex4,0,1,1087,477,105,132,0,WHITE); // popcorn cart
    drawSprite(backgroundTex3,0,1,1162,598,75,75,0,WHITE); // balloon booth

}
