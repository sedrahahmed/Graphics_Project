#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"
#include "Config.h"


//da for enemies
static unsigned int enemyWalkTexture[3] = { 0 }; //unsigned bec el lload texture returns it 
static unsigned int enemyAttackTexture [3]= {0};//load texture takes the pic and gives it to gpu and id
//Players

static unsigned int playerWalkTexture[3] = { 0 }; //unsigned bec el lload texture returns it
static unsigned int playerAttackTexture[3] = { 0 };
static unsigned int playerJumpTexture[2]={};
static unsigned int playerDuckTexture[2]={};
static unsigned int tokenTexture = 0;

static unsigned int sparkTexture = 0;
static unsigned int clownBallTexture = 0;
static unsigned int dartTexture = 0;

static unsigned int PlayerFrontTexture[3] = { 0 };

void drawGuardIcon(PlayerType type, float x, float y, float w,float h, glm::vec4 tint){
    drawSprite(PlayerFrontTexture[type],0,1, x, y, w, h,0,tint);
    
}

void drawMeleeSeif(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX, float scale) {
    unsigned int tex;
    int frames;
    int frame;
    switch(state){
        case ATTACK :{
            tex = playerAttackTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(stateTime * ATTACK_FRAMES_PER_SECOND) % 8;
            break;
        }
        case JUMP :{
            tex = playerJumpTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(stateTime / JUMP_DURATION * 8);              // plays once over the jump
            if (frame > 7) frame = 7;
            break;
        }
        case DUCK :{
            tex = playerDuckTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(stateTime / DUCK_DURATION * 8);
            if (frame > 7) frame = 7;
            break;
        }

        default :{
            tex = playerWalkTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(time * 8) % 8;
            break;
        }

    }
        float size = 160 * scale;   // scale > 1 only in the cutscene
        // the bottom 9.4% of seif's frames is empty, so when scaled pull him down by
        // the extra empty space to keep his feet where they are at normal size
        drawSprite(tex, frame, frames, x, y + size / 2 - 4 - (size - 160) * 0.094f, size, size, 0.0f,
        damageTint(WHITE, healthFrac), flipX);
        }


        
void drawRangedLama(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX) {
    unsigned int tex;
    int frames;
    int frame;
    switch(state){
        case ATTACK: {
            tex = playerAttackTexture[RANGED_LAMA];
            frames = 8;
            frame = (int)(stateTime * (8.0f / LANTERN_COOLDOWN) + 4) % 8;
            break;
        }
        case JUMP: {
            tex = playerJumpTexture[RANGED_LAMA];
            frames = 8;
            frame = (int)(stateTime / DUCK_DURATION * 8);
            if (frame > 7) frame = 7;
            break;
        }
        case DUCK: {
            tex = playerDuckTexture[RANGED_LAMA];
            frames = 8;
            frame = (int)(stateTime / DUCK_DURATION * 8);
            if (frame > 7) frame = 7;
            break;
        }
        default: {
            tex = playerWalkTexture[RANGED_LAMA];
            frames = 8;
            frame = (int)(time * 8) % 8;
            break;
        }
    }
    float size = 160;
    if (state == DUCK) size = 130;
    drawSprite(tex, frame, frames, x, y + size / 2-4, size, size, 0.0f,
    damageTint(WHITE, healthFrac), flipX);
}
void drawDefenseJana(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX) {
    unsigned int tex;
    int frame;
    if (state == WALK && playerWalkTexture[DEFENSE_JANA] != 0) {
        // only the cutscene ever puts Jana in WALK (beat 3/4 walk-ins);
        // gameplay never does, so nothing changes on the field.
        // jana_walk.png frames are tall (272 x 724) with lots of empty space:
        // she fills only ~35% of the frame height and her feet are ~31% up from the bottom.
        // So draw the frame at its real shape, scaled so she's as tall as in jana_defend.
        frame = (int)(time * 8) % 8;
        float h = 335.0f;                          // frame height on screen -> Jana ~118 px tall, like jana_defend
        float w = h * (272.0f / 724.0f);           // keep the frame's shape (no squishing)
        float centerY = y + 7.0f + h * (0.5f - 0.31f);   // feet land where jana_defend's feet are
        drawSprite(playerWalkTexture[DEFENSE_JANA], frame, 8, x, centerY, w, h, 0.0f,
                   damageTint(WHITE, healthFrac), flipX);
        return;
    } else {
        tex = playerAttackTexture[DEFENSE_JANA];    // jana_defend: bracing / standing
        if (state == ATTACK)                                  // bracing frames while blocked
            frame = (int)(stateTime * ATTACK_FRAMES_PER_SECOND) % 8;
        else
            frame = 0;                                        // first frame, standing still
    }
    float size = 140;
    drawSprite(tex, frame, 8, x, y + size / 2-4, size, size, 0.0f,
    damageTint(WHITE, healthFrac), flipX);
}

void drawGuard(float x, float y, PlayerType type, float time, AnimState state, float stateTime, float healthFrac, bool flipX, float scale) {
    switch (type) {
        case MELEE_SEIF:   drawMeleeSeif(x, y, time, state, stateTime, healthFrac, flipX, scale); break;
        case RANGED_LAMA:  drawRangedLama(x, y, time, state, stateTime, healthFrac, flipX); break;
        case DEFENSE_JANA:  drawDefenseJana(x, y, time, state, stateTime, healthFrac, flipX); break;
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

void drawDart(float x, float y, float time, float hitProgress) {
    int frame = (int)(time * 12) % 8;                     // speed lines flicker as it flies
    float w = 80.0f * (1.0f - hitProgress);               // shrinks on impact
    float h = 40.0f * (1.0f - hitProgress);               // keeps the frame's 2:1 shape
    glm::vec4 tint(1.0f, 1.0f, 1.0f, 1.0f - hitProgress); // and fades
    drawSprite(dartTexture, frame, 8, x, y+15, w, h, 0.0f, tint);
}  
void drawSpark(float x, float y, float time, float hitProgress){
    int frame = (int)(time * 10) % 4;                     // twinkles, 10 frames per second
    float size = 140.0f * (1.0f - hitProgress);           // shrinks to nothing on impact
    glm::vec4 tint(1.0f, 1.0f, 1.0f, 1.0f - hitProgress); // and fades out
    drawSprite(sparkTexture, frame, 4, x, y - 4, size, size, 0.0f, tint);
}
void drawJuggleBall(float x, float y, float time, float hitProgress){
    int frame = (int)(time * 12) % 4;                     // star turns as it rolls
    float size = 110.0f * (1.0f - hitProgress);
    glm::vec4 tint(1.0f, 1.0f, 1.0f, 1.0f - hitProgress);
    drawSprite(clownBallTexture, frame, 4, x, y+15, size, size, 0.0f, tint);
}


//Collectibles

void drawToken(float x, float y, float time) {
    float size = 24.0f;   // width and height on screen in pixels
    drawSprite(tokenTexture, 0, 1, x, y + size / 2, size, size, 0.0f, WHITE);
}

void drawTokenIcon(float x, float y, float size, glm::vec4 tint) {
    drawSprite(tokenTexture, 0, 1, x, y, size, size, 0.0f, tint);
}


//Enemies

void initCharacterTexture() {
    enemyWalkTexture[MAGICIAN] = loadTexture(ASSET_DIR "characters/magician_walking.png");
    enemyWalkTexture[CLOWN] = loadTexture(ASSET_DIR "characters/clown_walking.png");//asset dir da 3ashan el path y work mesh 3andy bas
    enemyWalkTexture[JESTER] = loadTexture(ASSET_DIR "characters/jester_walking.png");
    enemyAttackTexture[MAGICIAN] = loadTexture(ASSET_DIR "characters/magician_standing.png");
    enemyAttackTexture[CLOWN] = loadTexture(ASSET_DIR "characters/clown_standing.png");
    enemyAttackTexture[JESTER] = loadTexture(ASSET_DIR "characters/jester_standing.png");
    playerWalkTexture[MELEE_SEIF] = loadTexture(ASSET_DIR "characters/seif_walk.png");
    playerWalkTexture[RANGED_LAMA] = loadTexture(ASSET_DIR "characters/lama_walk.png");
    playerWalkTexture[DEFENSE_JANA] = loadTexture(ASSET_DIR "characters/jana_walk.png");
    playerAttackTexture[MELEE_SEIF] = loadTexture(ASSET_DIR "characters/seif_attack.png");
    playerAttackTexture[RANGED_LAMA] = loadTexture(ASSET_DIR "characters/lama_attack.png");  
    playerAttackTexture[DEFENSE_JANA] = loadTexture(ASSET_DIR "characters/jana_defend.png");
    playerJumpTexture[MELEE_SEIF] = loadTexture(ASSET_DIR "characters/seif_jump.png"); 
    playerJumpTexture[RANGED_LAMA] = loadTexture(ASSET_DIR "characters/lama_jump.png"); 
    playerDuckTexture[MELEE_SEIF] = loadTexture(ASSET_DIR "characters/seif_duck.png"); 
    playerDuckTexture[RANGED_LAMA] = loadTexture(ASSET_DIR "characters/lama_duck.png"); 
    sparkTexture = loadTexture(ASSET_DIR "characters/magician_spell.png");
    clownBallTexture  = loadTexture(ASSET_DIR "characters/clown_ball.png");
    PlayerFrontTexture[MELEE_SEIF] = loadTexture(ASSET_DIR "characters/seif_front.png");
    PlayerFrontTexture[RANGED_LAMA] = loadTexture(ASSET_DIR "characters/lama_front.png");
    PlayerFrontTexture[DEFENSE_JANA] = loadTexture(ASSET_DIR "characters/jana_front.png");
    dartTexture = loadTexture(ASSET_DIR "characters/lama_dart.png");
    tokenTexture = loadTexture(ASSET_DIR "background/Token.png");

}

void drawMagician(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
   
  
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[MAGICIAN] : enemyWalkTexture[MAGICIAN];
    float t=(state==ATTACK)?stateTime:time;//time da 3ady
 if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*ENEMY_FRAMES_PER_SECOND) % 8; //i did el seconds le how many frmaes passed then make it int bas
        float size=140.0f;
        drawSprite(tex, frameCount, 8, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));





}
void drawClown(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[CLOWN] : enemyWalkTexture[CLOWN];
    float t=(state==ATTACK)?stateTime:time;//time da 3ady fa han loop bas state da starts 0 lama n atack
    if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*ENEMY_FRAMES_PER_SECOND) % 8; //i did el seconds le how many frmaes passed then make it int bas
        float size=140.0f;
        drawSprite(tex, frameCount, 8, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));
}
void drawJester(float x, float y, float time, AnimState state, float stateTime, float healthFrac) {
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[JESTER] : enemyWalkTexture[JESTER];
    float t=(state==ATTACK)?stateTime:time;//time da 3ady fa han loop bas state da starts 0 lama n atack
    if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*ENEMY_FRAMES_PER_SECOND) % 8; //i did el seconds le how many frmaes passed then make it int bas
        float size=140.0f;
        drawSprite(tex, frameCount, 8, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));
}


//Health bar
void drawHealthBar(float x, float y, int current, int max) {
    float w = 50.0f, h = 6.0f;
    float frac = max > 0 ? (float)current / max : 0.0f;
    if (frac < 0.0f) frac = 0.0f; else if (frac > 1.0f) frac = 1.0f;
    glm::vec4 fillcolor;
    if (frac > 0.6)
        fillcolor = HEALTHBAR_GREEN;  
    else if (frac > 0.3)
        fillcolor = HEALTHBAR_ORANGE;    
    else
        fillcolor = HEALTHBAR_RED;
    drawRect(x, y, w+4, h+4, 0, BLACK);
    drawCircle(x+w/2,y, (h+4)/2, BLACK);
    drawRect(x, y, w, h, 0, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    drawCircle(x+w/2,y, h/2, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    drawRect(x - w / 2 + (w * frac) / 2, y, w * frac, h, 0, fillcolor);
    if(frac >= 1)
        drawCircle(x+w/2,y, h/2, fillcolor);
    float shineW = w*frac*0.8;
    drawRect(x-w/2+2+shineW/2, y+h/4, shineW, h/4, 0, glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
    float left = x-w/2;
    float segmentWidth = w/max;
    for(int i=1; i<max;i++){
        float dx=left + i * segmentWidth;
        drawLine(dx, y-h/2, dx, y+h/2, 1,BLACK);
    }
    float hx=x-w/2;
    float hy=y;
    float s=h*2.2;
    drawCircle(hx - s/4, hy + s/6, s/4+ 1.5, BLACK);
    drawCircle(hx + s/4, hy + s/6, s/4+1.5, BLACK);
    drawTriangle(hx, hy - s/6, s+3, s * 0.6f+3, 180, BLACK);
    drawCircle(hx - s/4, hy + s/6, s/4, HEART_RED);
    drawCircle(hx + s/4, hy + s/6, s/4, HEART_RED);
    drawTriangle(hx, hy - s/6, s, s * 0.6f, 180, HEART_RED);
    drawRect(hx - s/4, hy + s/4, s/6, s/6, 0, WHITE);

}

