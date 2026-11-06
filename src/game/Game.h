#pragma once

#include <vector>
#include "Entities.h"
#include "Config.h"


//everything on the field and the main game loop ha keep it here
//  To get rid of smt set its alive to false and at the end of each frame el game will remove it
// extern is Created once in Game.cppwe el file that includes Game.h can read them
extern std::vector<Player>     players;  // guards that are placed
extern std::vector<Enemy>      enemies;   //enemies
extern std::vector<Projectile> shots;   // el projectiles
extern std::vector<Drop>       drops;//tokens
extern float gameTime;// for animations el seconds since round starts

void resetGame();  
void updateGame(float dt);    // every frame while playing bey run game logic
void drawGame();                 //bg eviroment home base lanes tkens players shots healthbar hud we cards

                             
float laneCenterY(int lane);  
float laneFloorY(int lane);   //y of the feet of characters

bool placeGuard(PlayerType type, int lane, float x = -1.0f);

void spawnEnemy(EnemyType type, int lane);

void fireShot(ShotType type, int lane, float x, bool fromPlayer);

void dropToken(int lane, float x);
