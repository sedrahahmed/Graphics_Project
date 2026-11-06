#include "CharacterDraw.h"
#include "Draw.h"
#include "Colors.h"
#include "Config.h"


//da for enemies
static unsigned int enemyWalkTexture[3] = { 0 }; //unsigned bec el lload texture returns it 
static unsigned int enemyAttackTexture [3]= {0};//load texture takes the pic and gives it to gpu and gives us id


//Players
static unsigned int playerWalkTexture[3] = { 0 }; //unsigned bec el load texture returns it
static unsigned int playerAttackTexture[3] = { 0 };
static unsigned int playerJumpTexture[2]={};
static unsigned int playerDuckTexture[2]={};//bec jana no jump or duck
static unsigned int tokenTexture = 0;
static unsigned int PlayerFrontTexture[3] = { 0 };

static unsigned int sparkTexture = 0;
static unsigned int clownBallTexture = 0;
static unsigned int dartTexture = 0;//one id



void drawGuardIcon(PlayerType type, float x, float y, float w,float h, glm::vec4 tint){
    drawSprite(PlayerFrontTexture[type],0,1, x, y, w, h,0,tint);//frame 0 of a 1 frame pic
    
}

void drawMeleeSeif(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX, float scale) {
    unsigned int tex;//pic
    int frames;//how many frames fwl pic
    int frame;//curr frame
    switch(state){
        case ATTACK :{
            tex = playerAttackTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(stateTime * ATTACK_FRAMES_PER_SECOND) % 8;//turn the statetime to which frame to show 
            break;
        }
        case JUMP :{
            tex = playerJumpTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(stateTime / JUMP_DURATION * 8);             
            if (frame > 7) frame = 7;
            break;
        }
        case DUCK :{
            tex = playerDuckTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(stateTime / DUCK_DURATION * 8);
            if (frame > 7) frame = 7;
            break;//sae as jump
        }

        default :{//walk
            tex = playerWalkTexture[MELEE_SEIF];
            frames = 8;
            frame = (int)(time * 8) % 8;
            break;//use time bce da el default 8 frames per sec
        }

    }
        float size = 160 * scale;   // scale makes him bigger (only the cutscene uses it)
        //alight feet to lane fa move el y
        drawSprite(tex, frame, frames, x, y + size / 2 - 4 - (size - 160) * 0.094f, size, size, 0.0f,
        damageTint(WHITE, healthFrac), flipX);//w and h are size in px yhen no rot . color sis red tint thing wel flip x mirrors
        }


    //similar idea    
void drawRangedLama(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX, float scale) {
    unsigned int tex;
    int frames;
    int frame;
    switch(state){
        case ATTACK: {
            tex = playerAttackTexture[RANGED_LAMA];
            frames = 8;
            frame = (int)(stateTime * LAMA_FRAMES_PER_SEC+4) % 8;

            break;
        }
        case JUMP: {
            tex = playerJumpTexture[RANGED_LAMA];
            frames = 8;
            frame = (int)(stateTime / JUMP_DURATION * 8);
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
             frame = (int)(time * LAMA_FRAMES_PER_SEC) % 8;
            break;
        }
    }
    float base = 160;
    if (state == DUCK) base = 130;
    float size = base * scale;   // scale makes her bigger (only the cutscene uses it)
    // move her downto keep her feet in place
    drawSprite(tex, frame, frames, x, y + size / 2-6 - (size - base) * 0.109f, size, size, 0.0f,
    damageTint(WHITE, healthFrac), flipX);
}
void drawDefenseJana(float x, float y, float time, AnimState state, float stateTime, float healthFrac, bool flipX, float scale) {
    unsigned int tex;
    int frame;
    if (state == WALK && playerWalkTexture[DEFENSE_JANA] != 0) { //only in the cutscene 
        frame = (int)(time * 8) % 8;
        float h = 335.0f * scale; // picture height on screen scale lel cutcene
        float w = h * (272.0f / 724.0f);   // keep frame shape
        float centerY = y + 7.0f + h * (0.5f - 0.31f);   // align feet
        drawSprite(playerWalkTexture[DEFENSE_JANA], frame, 8, x, centerY, w, h, 0.0f,
                   damageTint(WHITE, healthFrac), flipX);
        return;
    } else {
        tex = playerAttackTexture[DEFENSE_JANA];  
        if (state == ATTACK)        //defending frames
            frame = (int)(stateTime * ATTACK_FRAMES_PER_SECOND) % 8;
        else
            frame = 0;          // first frame standing still
    }
    float size = 140 * scale;   // scale makes her bigger for cutscene
   // move her down bc empty space underfeet
    // by the extra when she gets bigger to keep her feet in place
    drawSprite(tex, frame, 8, x, y + size / 2-4 - (size - 140) * 0.07f, size, size, 0.0f,
    damageTint(WHITE, healthFrac), flipX);
}

void drawGuard(float x, float y, PlayerType type, float time, AnimState state, float stateTime, float healthFrac, bool flipX, float scale) {
    switch (type) {
        case MELEE_SEIF:   drawMeleeSeif(x, y, time, state, stateTime, healthFrac, flipX, scale); break;
        case RANGED_LAMA:  drawRangedLama(x, y, time, state, stateTime, healthFrac, flipX, scale); break;
        case DEFENSE_JANA:  drawDefenseJana(x, y, time, state, stateTime, healthFrac, flipX, scale); break;
    }
}

void drawEnemy(float x, float y, EnemyType type, float time, AnimState state, float stateTime, float healthFrac, float scale) {
    switch (type) {
        case MAGICIAN: drawMagician(x, y, time, state, stateTime, healthFrac, scale); break;
        case CLOWN:    drawClown(x, y, time, state, stateTime, healthFrac, scale);    break;
        case JESTER:   drawJester(x, y, time, state, stateTime, healthFrac, scale);   break;
    }
}

//Projectiles

void drawDart(float x, float y, float time, float hitProgress) {//0 to 1 finishes hit fel 1
    int frame = (int)(time * 12) % 8;  //pick frame to show                  
    float w = 80.0f * (1.0f - hitProgress); // shrinks on impact
    float h = 40.0f * (1.0f - hitProgress);     // keeps the frames shape
    glm::vec4 tint(1.0f, 1.0f, 1.0f, 1.0f - hitProgress); //fades
    drawSprite(dartTexture, frame, 8, x, y+15, w, h, 0.0f, tint);
}  //same idea
void drawSpark(float x, float y, float time, float hitProgress){
    int frame = (int)(time * 10) % 4;                     //10 frames per second
    float size = 140.0f * (1.0f - hitProgress);          
    glm::vec4 tint(1.0f, 1.0f, 1.0f, 1.0f - hitProgress);
    drawSprite(sparkTexture, frame, 4, x, y - 4, size, size, 0.0f, tint);
}
void drawJuggleBall(float x, float y, float time, float hitProgress){
    int frame = (int)(time * 12) % 4;                     // star turns as it rolls
    float size = 110.0f * (1.0f - hitProgress);
    glm::vec4 tint(1.0f, 1.0f, 1.0f, 1.0f - hitProgress);
    drawSprite(clownBallTexture, frame, 4, x, y+15, size, size, 0.0f, tint);
}


//Collectibles
void drawToken(float x, float y) {
    float size = 24.0f;   // width and height on screen in pixels
    drawSprite(tokenTexture, 0, 1, x, y + size / 2, size, size, 0.0f, WHITE);
}

void drawTokenIcon(float x, float y, float size, glm::vec4 tint) {
    drawSprite(tokenTexture, 0, 1, x, y, size, size, 0.0f, tint);
}




void initCharacterTexture() {
    enemyWalkTexture[MAGICIAN] = loadTexture(ASSET_DIR "characters/magician_walking.png");
    enemyWalkTexture[CLOWN] = loadTexture(ASSET_DIR "characters/clown_walking.png");//asset dir da el full path lel assets folder
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

void drawMagician(float x, float y, float time, AnimState state, float stateTime, float healthFrac, float scale) {
   
  
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[MAGICIAN] : enemyWalkTexture[MAGICIAN];//atttack el stand while attack el walk attack while walk
    float t=(state==ATTACK)?stateTime:time;//time da game clock law statetime its 0 awel man attack start frame 0
 if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*ENEMY_FRAMES_PER_SECOND) % 8; //curr frame ashoof ana emta then frames per sec to see show which frame mod 8 to wrap
        float size=140.0f*scale;//scale makes them bigger (only the cutscene uses it)
        drawSprite(tex, frameCount, 8, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));





}
void drawClown(float x, float y, float time, AnimState state, float stateTime, float healthFrac, float scale) {
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[CLOWN] : enemyWalkTexture[CLOWN];
    float t=(state==ATTACK)?stateTime:time;//time da game clock law statetime its 0 awel man attack start frame 0
    if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*ENEMY_FRAMES_PER_SECOND) % 8;//curr frame ashoof ana emta then frames per sec to see show which frame mod 8 to wrap
        float size=140.0f*scale;   // scale makes them bigger (only the cutscene uses it)
        drawSprite(tex, frameCount, 8, x, y+size/2, size, size, 0.0f, damageTint(  WHITE, healthFrac));
}
void drawJester(float x, float y, float time, AnimState state, float stateTime, float healthFrac, float scale) {
    unsigned int tex = (state == ATTACK) ? enemyAttackTexture[JESTER] : enemyWalkTexture[JESTER];
    float t=(state==ATTACK)?stateTime:time;//time da game clock law statetime its 0 awel man attack start frame 0
    if(tex==0){
        drawRect(x, y + 40, 40, 80, 0, damageTint(PLACEHOLDER_GRAY, healthFrac));   // di el grey box test law el sooora bazet or smt
        return;}
        int frameCount = (int) (t*ENEMY_FRAMES_PER_SECOND) % 8; //curr frame ashoof ana emta then frames per sec to see show which frame mod 8 to wrap
        float size=154.0f*scale;   // scale makes them bigger (only the cutscene uses it)
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

