// Game.cpp. Owner: Person C
#include "Game.h"
#include "CharacterDraw.h"
#include "Draw.h"
#include "Lanes.h"
#include "HUD.h"
#include "Economy.h"
#include "Waves.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include "StateManager.h"
#include "Combat.h"
#include "Sound.h"
#include "Background.h"
#include "Environment.h"
#include "Shots.h"
#include "PlayerCards.h"



std::vector<Player>     players;
std::vector<Enemy>      enemies;
std::vector<Projectile> shots;
std::vector<Drop>       drops;
float gameTime = 0.0f;//counts seconds the game has been running for 

//helpers
static int maxHealthOf(PlayerType type) {
    switch(type) {
        case MELEE_SEIF:    return HP_MELEE;
        case RANGED_LAMA:   return HP_RANGED;
        case DEFENSE_JANA:  return HP_DEFENSE;
        default:              return 0;
    }
}

static int maxHealthEnemy(EnemyType type) {
    
    switch(type) {
        case CLOWN:       return HP_CLOWN;
        case JESTER:   return HP_JESTER;
        case MAGICIAN:  return HP_MAGICIAN;
        default:              return 0;
    }
}
   

static void moveEveryone(float dt) {
    for(Player& p: players){
        p.stateTimer += dt;
        if(p.state == JUMP && p.stateTimer >= JUMP_DURATION) {//if jump lasted its duration go abck to wlaking
            p.state = WALK;
            p.stateTimer = 0;
        }
        if(p.state == DUCK && p.stateTimer >= DUCK_DURATION) {
            p.state = WALK;
            p.stateTimer = 0;
        }
        if(p.state == WALK) {
            p.x += SPEED_GUARD * dt;//distance speedxtime and using dt da makes it same speed for kol el computers
        }
        if(p.x > LANE_RIGHT) {
            p.alive = false; // remove the guard if it walks off the right edge of the screen
        }
    }
    for(Enemy& e: enemies){
        e.stateTimer += dt;
        if(e.state== WALK){
        e.x -= (e.type==JESTER)?SPEED_JESTER * dt : SPEED_ENEMY * dt;//move left but check if jester bec hes quicker
        }
    }
 } 
    
static void checkWinLose()    {
for(Enemy& e: enemies){
    if(e.x <= LANE_LEFT && e.alive) {
        changeState(LOST);
        return;
    }


}
if(allWavesSpawned() && enemies.empty()) {
    changeState(WON);
    return;
 }
}

static void removeDead()      { 
    for(int i=(int)players.size()-1; i>=0; i--) { //typecast 3ashan signed and unsigned moshkela and loop backward bec el shifting fel delete yezbot
        if(!players[i].alive) {
            players.erase(players.begin() + i);//bec i gets ane rror erase needs marker not int
        
        }
    }

 for(int i=(int)enemies.size()-1; i>=0; i--) { //typecast 3ashan signed and unsigned moshkela
        if(!enemies[i].alive) {
            enemies.erase(enemies.begin() + i);
        
        }
    }

     for(int i=(int)shots.size()-1; i>=0; i--) { //typecast 3ashan signed and unsigned moshkela
        if(!shots[i].alive) {
            shots.erase(shots.begin() + i);
        
        }
    }

     for(int i=(int)drops.size()-1; i>=0; i--) { //typecast 3ashan signed and unsigned moshkela
        if(!drops[i].alive) {
            drops.erase(drops.begin() + i);
        
        }
    }


}
static AttackHeight heightOf(ShotType type) {
    
    switch(type) {
        case LAMA_DART: return HIGH;//ik enemies cant dodge bas saybaha
        case MAGICIAN_HIT: return HIGH;
        case JUGGLE_BALL: return LOW;
        default:          return MELEE_HIT;
    }
}



void resetGame() { 
    players.clear();
    enemies.clear();
    shots.clear();
    drops.clear();
    gameTime = 0.0f;
    resetEconomy();
    resetWaves();   
    resetLanes();

}

void updateGame(float dt) {
    gameTime += dt;//akenaha clock
    updatePlayerCards();
    updateLanes();//hovering clicking etc
         
    updateWaves(dt);
    moveEveryone(dt);
    updateCombat(dt);
    updateShots(dt);
    updateEconomy();
    checkWinLose();
    removeDead();
   
    
}

void drawGame() {
   
    drawBackground(gameTime);
    drawEnvironment();
    drawLanes();
    for(const Drop& d : drops) {
        drawToken(d.x, laneFloorY(d.lane), gameTime);
    }
    for (const Player& p : players) {
                float cooldown = HERO_ATTACK_COOLDOWN;
        float animTime = (p.state == ATTACK) ? (cooldown - p.attackTimer) : p.stateTimer;//if attacking right after an attack attack timer is 1.5 fa we draaw frame 0 and so on 

        drawGuard(p.x, laneFloorY(p.lane), p.type, gameTime, p.state, animTime, (float)p.health / p.maxHealth);//last argument for the red shades and dec health
        drawHealthBar(p.x, laneFloorY(p.lane) + 140, p.health, p.maxHealth); 
    }
    for (const Enemy& e : enemies) {
        drawEnemy(e.x, laneFloorY(e.lane), e.type, gameTime, e.state, e.stateTimer, (float)e.health / e.maxHealth);
        drawHealthBar(e.x, laneFloorY(e.lane) + 140, e.health, e.maxHealth); 
    }
    for (const Projectile& s : shots) {
    float floorY = laneFloorY(s.lane);   // the floor of the shot's lane
        float progress = s.hitting ? s.hitTimer / HIT_DURATION : 0.0f; // 0 = flying 1 = finished
    switch (s.type) {
        case MAGICIAN_HIT:   //head height so ducking dodges it. tried until zabatet
            drawSpark(s.x, floorY + 140 -56, gameTime, progress);
            break;
        case JUGGLE_BALL:   // clown ball on the floor so jumping dodges it
            drawJuggleBall(s.x, floorY + 12.0f, gameTime, progress);
            break;
        case LAMA_DART:   //Lamas dart about chest height keda
            drawDart(s.x, floorY + 140 -84, gameTime, progress);
            break;
    }
    }
    drawHUD(getCoins(), getCurrentWave());
    drawPlayerCards();
}

float laneCenterY(int lane) { return LANE_BOTTOM + LANE_HEIGHT * lane + LANE_HEIGHT / 2.0f; }//get where each ones bottom is then add half height
float laneFloorY(int lane)  { return LANE_BOTTOM + LANE_HEIGHT * lane + 10.0f; }
//initialiize our 4 arrays
bool placeGuard(PlayerType type, int lane, float x) {
    if(!spend(type)) 
        return false; //if the player can't afford the guard ha return false
    Player newPlayer;
    newPlayer.type = type;
    newPlayer.lane = lane;
    newPlayer.x = LANE_LEFT + 40.0f; //Place the guard at the left edge of the lane
    if (type == DEFENSE_JANA && x >= 0) {  // Defense Guard goes where the mouse clicked
        newPlayer.x = x;
        if (newPlayer.x < LANE_LEFT + 40.0f)  newPlayer.x = LANE_LEFT + 40.0f;    // keep it fully inside the lane and visisble
        if (newPlayer.x > LANE_RIGHT - 40.0f) newPlayer.x = LANE_RIGHT - 40.0f;
    }
    newPlayer.maxHealth = maxHealthOf(type);
    newPlayer.health = newPlayer.maxHealth;
    newPlayer.state = (newPlayer.type == DEFENSE_JANA) ? IDLE : WALK; //Defense guards start idle el ba2y start walking
    newPlayer.stateTimer = 0.0f;
    newPlayer.attackTimer = 0.0f; //attack immediatly
    newPlayer.alive = true;
    players.push_back(newPlayer);//add to list of players
    playSoundEffect(SOUND_PLACEMENT);
    return true;
}

    
void spawnEnemy(EnemyType type, int lane) { 
Enemy e;

    e.type = type;
    e.lane = lane;
    e.x = LANE_RIGHT; //Place the enemy at the right edge of the lane.
    e.maxHealth = maxHealthEnemy(type);
    e.health = e.maxHealth;
    e.state = WALK; //Enemies start walking
    e.stateTimer = 0.0f;
    e.attackTimer = 0.0f; //attack immediatly
    e.alive = true;
    enemies.push_back(e);



}
void fireShot(ShotType type, int lane, float x, bool fromPlayer) { 
    Projectile p;
    p.type = type;
    p.lane = lane;
    p.x = x;
    p.fromPlayer = fromPlayer;
    p.height = heightOf(type);
    p.hitting = false;
    p.hitTimer = 0.0f;
    p.alive = true;
    shots.push_back(p);
}
void dropToken(int lane, float x) { 
    Drop d;
    d.lane=lane;
    d.x=x;
    d.alive=true;
    drops.push_back(d);
}
