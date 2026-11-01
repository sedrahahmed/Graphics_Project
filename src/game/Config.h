#pragma once
//ha name el numbers hena 3ashan nefham and if we want to change smt from here bas

const int   SCREEN_W = 1280;//screen width of height the origin  bottom left top right is these coordinates pixels
const int   SCREEN_H = 720;

const int   LANE_COUNT = 4;
const float LANE_BOTTOM = 40.0f;      // y where the lowest lane starts
const float LANE_HEIGHT = 130.0f;     // each lane is 130 px tall
const float LANE_LEFT = 160.0f;     // lanes start right of the home base be shwaya
const float LANE_RIGHT = 1280.0f;    // enemies enter men hena

//ASK ABOUT THIS
// Lane 0 is the bottom lane. Center of lane i:
//   LANE_BOTTOM + LANE_HEIGHT * i + LANE_HEIGHT / 2

const float HOME_BASE_X = 80.0f;      // center of the  booth(homebase)
const float HUD_Y = 600.0f;     // cards + coin count are fo2 da 


const int START_COINS = 30;
const int COST_MELEE = 10;  // Mirror Guard
const int COST_RANGED = 20;    // Lantern Guard 
const int COST_DEFENSE = 30;   // Sentinel Guard
const int COINS_PER_KILL = 10;

const int HP_MELEE = 5, HP_RANGED = 5, HP_DEFENSE = 7;
const int HP_MAGICIAN = 5, HP_CLOWN = 5, HP_JESTER = 7;


const int DMG_PLAYER = 1;
const int DMG_ENEMY = 1;
const int DMG_JESTER = 2;


const float SPEED_GUARD = 40.0f;
const float SPEED_ENEMY = 30.0f;
const float SPEED_JESTER = 70.0f;   // bec da hayegry
const float SPEED_PROJECTILE = 250.0f;


const float ATTACK_COOLDOWN = 1.0f;    
const float JUMP_DURATION = 0.6f;
const float DUCK_DURATION = 0.6f;


const int WAVE_SIZES[3] = { 3, 5, 10 };
const float SPAWN_INTERVAL = 4.0f;    // time between the enemies lama yetla3o