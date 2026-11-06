#include "Environment.h"
#include "Draw.h"
#include "Colors.h"

static unsigned int balloon_booth = 0;
static unsigned int circus_tent = 0;
static unsigned int moon = 0;
static unsigned int popcorn_cart = 0;
static unsigned int carousel = 0;

//loads the images for the 5 primitives
void initEnvironment() {
    balloon_booth = loadTexture(ASSET_DIR "background/balloon_booth.png");
    circus_tent = loadTexture(ASSET_DIR "background/circus_tent.png");
    moon = loadTexture(ASSET_DIR "background/Moon.png");
    popcorn_cart = loadTexture(ASSET_DIR "background/popcorn_cart_with_flag.png");
    carousel = loadTexture(ASSET_DIR "background/carousel.png");
}

//5 primitives for the background
// widths follow each PNG's shape (height kept) so nothing is stretched.
// y is picked so each prop's visible bottom stands at y = 400 (on the boardwalk lane)
void drawEnvironment() {
    drawSprite(balloon_booth,0,1,271,476.7f,143,157,0,WHITE);
    drawSprite(circus_tent,0,1,676,479.0f,198,163,0,WHITE);
    drawSprite(carousel,0,1,896,469.7f,164,154,0,WHITE); //carousel
    drawSprite(popcorn_cart,0,1,1087,460.5f,120,132,0,WHITE); //popcorn cart
    drawSprite(moon,0,1,1162,598,75,75,0,WHITE);
}
