#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"
#include "Config.h"

//Players
void drawMirrorGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawLanternGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 35, 40, 70, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
}
void drawDefenseGuard(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    drawRect(x, y + 50, 60, 100, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));  // TODO
}

void drawGuard(float x, float y, PlayerType type, float time, AnimState state, float stateTime, float healthFrac) {
    switch (type) {
        case MIRROR_GUARD:   drawMirrorGuard(x, y, time, state, stateTime, healthFrac); break;
        case LANTERN_GUARD:  drawLanternGuard(x, y, time, state, stateTime, healthFrac); break;
        case DEFENSE_GUARD:  drawDefenseGuard(x, y, time, state, stateTime, healthFrac); break;
    }
}

void drawEnemy(float x, float y, EnemyType type, float time, AnimState state, float stateTime, float healthFrac) {
    switch (type) {
        case MAGICIAN: drawMagician(x, y, time, state, stateTime, healthFrac); break;
        case CLOWN:    drawClown(x, y, time, state, stateTime, healthFrac);    break;
        case JESTER:   drawJester(x, y, time, state, stateTime, healthFrac);   break;
    }
}

//Projectiles

void drawLanternOrb(float x, float y, float hitProgress) { drawCircle(x, y, 8, GHOST_LIGHT); }   // TODO
void drawCursedCard(float x, float y, float hitProgress) { drawRect(x, y, 12, 16, 0, WHITE); }   // TODO
void drawJuggleBall(float x, float y, float hitProgress) { drawCircle(x, y, 8, CURSE_RED); }     // TODO


//Collectibles

void drawToken(float x, float y, float time) { drawCircle(x, y, 10, TICKET_GOLD); }   // TODO


//Enemies
static unsigned int enemyWalkTexture[3] = { 0 }; //unsigned bec el lload texture returns it 
static unsigned int enemyAttackTexture [3]= {0};
void initCharacterTexture() {
    enemyWalkTexture[MAGICIAN] = loadTexture(ASSET_DIR "characters/magician_walk.png");
    enemyWalkTexture[CLOWN] = loadTexture(ASSET_DIR "characters/clown_walk.png");//asset dir da 3ashan el path y work mesh 3andy bas
    enemyWalkTexture[JESTER] = loadTexture(ASSET_DIR "characters/jester_walk.png");
    enemyAttackTexture[MAGICIAN] = loadTexture(ASSET_DIR "characters/magician_attack.png");
    enemyAttackTexture[CLOWN] = loadTexture(ASSET_DIR "characters/clown_attack.png");
    enemyAttackTexture[JESTER] = loadTexture(ASSET_DIR "characters/jester_attack.png");
}

void drawMagician(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
   
    drawRect(x, y + 45, 40, 90, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // TODO
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[MAGICIAN] : enemyWalkTexture[MAGICIAN];
    float t=(state==ATTACK)?stateTime:time;//time da 3ady
 if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*Walk_ANIMATION_SPEED) % 4; //i did el seconds le how many frmaes passed then make it int bas
        float size=110.0f;
        drawSprite(tex, frameCount, 4, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));





}
void drawClown(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[CLOWN] : enemyWalkTexture[CLOWN];
    float t=(state==ATTACK)?stateTime:time;//time da 3ady fa han loop bas state da starts 0 lama n atack
    if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*Walk_ANIMATION_SPEED) % 4; //i did el seconds le how many frmaes passed then make it int bas
        float size=110.0f;
        drawSprite(tex, frameCount, 4, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));
}
void drawJester(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[JESTER] : enemyWalkTexture[JESTER];
    float t=(state==ATTACK)?stateTime:time;//time da 3ady fa han loop bas state da starts 0 lama n atack
    if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*Walk_ANIMATION_SPEED*1.5) % 4; //i did el seconds le how many frmaes passed then make it int bas
        float size=110.0f;
        drawSprite(tex, frameCount, 4, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));
}


//Health bar
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

