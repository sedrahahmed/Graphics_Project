#pragma once
// Game.h: the game world: everything on the field + the main game loop.
// Owner: Person C. Shared contract: don't change without telling the team.

#include <vector>
#include "Entities.h"
#include "Config.h"

// =====================================================================
// THE FOUR LISTS: everything currently on the field
// Created once in Game.cpp. Any file that includes Game.h can read them.
//
// RULE: never remove items yourself.
//   To get rid of something, set its "alive" to false.
//   Game removes everything that's not alive at the end of each frame.
// =====================================================================
extern std::vector<Player>     players;   // guards on the field
extern std::vector<Enemy>      enemies;   // cursed performers on the field
extern std::vector<Projectile> shots;     // orbs, cards and juggling balls in the air
extern std::vector<Drop>       drops;     // tokens waiting to be picked up

// Seconds since this round started. Used for animations.
// Stops while paused, so everything freezes.
extern float gameTime;

// =====================================================================
// MAIN FUNCTIONS: called by StateManager (Person A)
// =====================================================================
void resetGame();             // empty all lists, coins back to start, wave 1, gameTime = 0
void updateGame(float dt);    // every frame while PLAYING: runs all game logic in order
void drawGame();              // draws the whole field, back to front
                              // (PAUSED / LOST: call drawGame only, not updateGame)

// =====================================================================
// LANE POSITIONS: use these instead of doing lane math yourself
// Lane 0 is the bottom lane.
// =====================================================================
float laneCenterY(int lane);  // y of the middle of the lane
float laneFloorY(int lane);   // y where characters' feet stand in the lane

// =====================================================================
// ADDING THINGS TO THE FIELD
// =====================================================================

// Called by Lanes (Person B) when a lane is clicked with a card selected.
// Checks coins, pays, adds the guard at the left of the lane.
// Returns false if it can't be afforded (nothing happens).
bool placeGuard(PlayerType type, int lane);

// Called by Waves (Person D). Adds a villain at the right edge, full health.
void spawnEnemy(EnemyType type, int lane);

// Called by Combat (Person C) when a ranged attacker fires.
// x = where the shooter is standing.
void fireShot(ShotType type, int lane, float x, bool fromPlayer);

// Called by Combat (Person C) when a villain dies.
void dropToken(int lane, float x);
