// Game.cpp. Owner: Person C
#include "Game.h"
#include "CharacterDraw.h"
#include "Draw.h"
#include "Lanes.h"

std::vector<Player>     players;
std::vector<Enemy>      enemies;
std::vector<Projectile> shots;
std::vector<Drop>       drops;
float gameTime = 0.0f;

void resetGame() { }

void updateGame(float dt) {
    // TODO, in this order:
    // gameTime += dt;
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
    drawLanes();
}

float laneCenterY(int lane) { return LANE_BOTTOM + LANE_HEIGHT * lane + LANE_HEIGHT / 2.0f; }
float laneFloorY(int lane)  { return LANE_BOTTOM + LANE_HEIGHT * lane + 20.0f; }

bool placeGuard(PlayerType type, int lane) { return false; }
void spawnEnemy(EnemyType type, int lane) { }
void fireShot(ShotType type, int lane, float x, bool fromPlayer) { }
void dropToken(int lane, float x) { }
