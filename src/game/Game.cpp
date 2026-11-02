// Game.cpp. Owner: Person C
#include "Game.h"
#include "CharacterDraw.h"
#include "Draw.h"
#include "Lanes.h"
#include "HUD.h"
#include "Economy.h"
#include "Waves.h"
#include "HomeBase.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include "StateManager.h"

std::vector<Player>     players;
std::vector<Enemy>      enemies;
std::vector<Projectile> shots;
std::vector<Drop>       drops;
float gameTime = 0.0f;

//helpers:
static int maxHealthOf(PlayerType type) {
    switch(type) {
        case MIRROR_GUARD:    return HP_MELEE;
        case LANTERN_GUARD:   return HP_RANGED;
        case DEFENSE_GUARD:  return HP_DEFENSE;
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
        if(p.state == JUMP && p.stateTimer >= JUMP_DURATION) {
            p.state = WALK;
            p.stateTimer = 0;
        }
        if(p.state == DUCK && p.stateTimer >= DUCK_DURATION) {
            p.state = WALK;
            p.stateTimer = 0;
        }
        if(p.state == WALK) {
            p.x += SPEED_GUARD * dt;
        }
        if(p.x > LANE_RIGHT) {
            p.alive = false; // remove the guard if it walks off the right edge of the screen
        }
    }
    for(Enemy& e: enemies){
        e.stateTimer += dt;
        if(e.state== WALK){
        e.x -= (e.type==JESTER)?SPEED_JESTER * dt : SPEED_ENEMY * dt;
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
    for(int i=(int)players.size()-1; i>=0; i--) { //typecast 3ashan signed and unsigned moshkela
        if(!players[i].alive) {
            players.erase(players.begin() + i);
        
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
    // TODO, in this order:
    gameTime += dt;
    updateLanes();
    if (keyPressed(GLFW_KEY_1) && getHoveredLane() != -1) placeGuard(MIRROR_GUARD, getHoveredLane());
    if (keyPressed(GLFW_KEY_2) && getHoveredLane() != -1) placeGuard(LANTERN_GUARD, getHoveredLane());
    if (keyPressed(GLFW_KEY_3) && getHoveredLane() != -1) placeGuard(DEFENSE_GUARD, getHoveredLane());
    if (keyPressed(GLFW_KEY_4) && getHoveredLane() != -1) spawnEnemy(MAGICIAN, getHoveredLane());
    if (keyPressed(GLFW_KEY_5) && getHoveredLane() != -1) spawnEnemy(CLOWN, getHoveredLane());
    if (keyPressed(GLFW_KEY_6) && getHoveredLane() != -1) spawnEnemy(JESTER, getHoveredLane());
    if (keyPressed(GLFW_KEY_7)) {
    for (Enemy& e : enemies) {
        e.alive = false;
    }}
    updateWaves(dt);
    moveEveryone(dt);
    checkWinLose();
    removeDead();
    // updateCombat(dt);
    // updateShots(dt); updateEconomy(dt); updatePlayerCards();
   
    
}

void drawGame() {
    // TODO, back to front:
    // drawBackground, drawEnvironment, drawHomeBase, drawLanes, tokens,
    // guards, villains, shots, health bars, drawHUD, drawPlayerCards
    drawHomeBase(gameTime);
    drawLanes();
    for (const Player& p : players) {
        drawGuard(p.x, laneFloorY(p.lane), p.type, gameTime, p.state, p.stateTimer, (float)p.health / p.maxHealth);
        drawHealthBar(p.x, laneFloorY(p.lane) + 115, p.health, p.maxHealth); 
    }
    for (const Enemy& e : enemies) {
        drawEnemy(e.x, laneFloorY(e.lane), e.type, gameTime, e.state, e.stateTimer, (float)e.health / e.maxHealth);
        drawHealthBar(e.x, laneFloorY(e.lane) + 115, e.health, e.maxHealth); 
    }
    drawHUD(getCoins(), getCurrentWave());
    //drawHealthBar(400, 650, 3, 5);
    //drawHealthBar(600, 650, 7, 7);
    //drawHealthBar(800, 650, 1, 5);
}

float laneCenterY(int lane) { return LANE_BOTTOM + LANE_HEIGHT * lane + LANE_HEIGHT / 2.0f; }
float laneFloorY(int lane)  { return LANE_BOTTOM + LANE_HEIGHT * lane + 20.0f; }

bool placeGuard(PlayerType type, int lane) { 
    if(!spend(type)) 
        return false; //If the player can't afford the guard, return false.
    Player newPlayer;
    newPlayer.type = type;
    newPlayer.lane = lane;
    newPlayer.x = LANE_LEFT + 40.0f; //Place the guard at the left edge of the lane. 
    newPlayer.maxHealth = maxHealthOf(type);
    newPlayer.health = newPlayer.maxHealth;
    newPlayer.state = (newPlayer.type == DEFENSE_GUARD) ? IDLE : WALK; //Defense guards start idle, others start walking.
    newPlayer.stateTimer = 0.0f;
    newPlayer.attackTimer = ATTACK_COOLDOWN; //Set the attack timer to the cooldown value.
    newPlayer.alive = true;
    players.push_back(newPlayer);
    return true;
}

    
void spawnEnemy(EnemyType type, int lane) { 
Enemy e;

    e.type = type;
    e.lane = lane;
    e.x = LANE_RIGHT; //Place the enemy at the right edge of the lane.
    e.maxHealth = maxHealthEnemy(type);
    e.health = e.maxHealth;
    e.state = WALK; //Enemies start walking.
    e.stateTimer = 0.0f;
    e.attackTimer = ATTACK_COOLDOWN; //Set the attack timer to the cooldown value.
    e.alive = true;
    enemies.push_back(e);



}
void fireShot(ShotType type, int lane, float x, bool fromPlayer) {}
void dropToken(int lane, float x) { }
