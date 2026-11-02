// Game.cpp. Owner: Person C
#include "Game.h"
#include "CharacterDraw.h"
#include "Draw.h"
#include "Lanes.h"
#include "HUD.h"
#include "Economy.h"
#include "Waves.h"
#include "HomeBase.h"
#include "Economy.h"
#include "Players.h"

std::vector<Player>     players;
std::vector<Enemy>      enemies;
std::vector<Projectile> shots;
std::vector<Drop>       drops;
float gameTime = 0.0f;

void resetGame() { }

void updateGame(float dt) {
    // TODO, in this order:
    gameTime += dt;
    // updateLanes(); updateWaves(dt); move everyone; updateCombat(dt);
    // updateShots(dt); updateEconomy(dt); updatePlayerCards();
    // check win/lose -> changeState(WON / LOST);
    // remove everything with alive == false
    updateLanes();
}

void drawGame() {
    // TODO, back to front:
    // drawBackground, drawEnvironment, drawHomeBase, drawLanes, tokens,
    // guards, villains, shots, health bars, drawHUD, drawPlayerCards
    drawHomeBase(gameTime);
    drawLanes();
    drawGuards();
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
    newPlayer.health = maxHealthOf(type);
    newPlayer.state = (newPlayer.type == DEFENSE_GUARD) ? IDLE : WALK; //Defense guards start idle, others start walking.
    newPlayer.stateTimer = 0.0f;
    newPlayer.attackTimer = ATTACK_COOLDOWN; //Set the attack timer to the cooldown value.
    newPlayer.alive = true;
    players.push_back(newPlayer);
    return true;
}
static int maxHealthOf(PlayerType type) {
    switch(type) {
        case MIRROR_GUARD:    return HP_MELEE;
        case LANTERN_GUARD:   return HP_RANGED;
        case DEFENSE_GUARD:  return HP_DEFENSE;
        default:              return 0;
    }
}
    
void spawnEnemy(EnemyType type, int lane) { }
void fireShot(ShotType type, int lane, float x, bool fromPlayer) { }
void dropToken(int lane, float x) { }
