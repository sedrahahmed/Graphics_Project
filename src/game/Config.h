#pragma once
//ha name el numbers hena 3ashan nefham and if we want to change smt from here bas

const int   SCREEN_W = 1280;//screen width of height the origin  bottom left top right is these coordinates pixels
const int   SCREEN_H = 720;

const int   LANE_COUNT = 4;
const float LANE_BOTTOM = 26.0f;      // y where the lowest lane starts
const float LANE_HEIGHT = 93.7f;     // each lane is 93.7 px tall
const float LANE_LEFT = 209.0f;     // lanes start right of the home base be shwaya
const float LANE_RIGHT = 1280.0f;    // enemies enter men hena


const int START_COINS = 30;
const int COST_MELEE = 10;  //seif
const int COST_RANGED = 20;    // lama 
const int COST_DEFENSE = 30;   // jana
const int COINS_PER_KILL = 20; //azon nezawedha 
const float HIT_DURATION = 0.2f;

const int HP_MELEE = 5, HP_RANGED = 5, HP_DEFENSE = 7;
const int HP_MAGICIAN = 5, HP_CLOWN = 5, HP_JESTER = 7;


const int DMG_PLAYER = 1;
const int DMG_ENEMY = 1;
const int DMG_JESTER = 2;


const float SPEED_GUARD = 40.0f;
const float SPEED_ENEMY = 30.0f;
const float SPEED_JESTER = 70.0f;   // bec da hayegry
const float SPEED_PROJECTILE = 250.0f;
const float SPEED_DART  = 250.0f;//lama
const float SPEED_BALL  = 250.0f; //clown
const float SPEED_SPARK = 220.0f;//magician


const float ENEMY_ATTACK_COOLDOWN = 4.0f; 
const float HERO_ATTACK_COOLDOWN = 1.5f;   // time between attacks for guards and enemies    
const float JUMP_DURATION = 0.6f;
const float DUCK_DURATION = 0.6f;
const float ATTACK_FRAMES_PER_SECOND = 8.0f / HERO_ATTACK_COOLDOWN;
const float ENEMY_FRAMES_PER_SECOND  = 8.0f / ENEMY_ATTACK_COOLDOWN;//how fast el frames hat play like thats why 8 frames over el time mabein el attacks nel7a2 n loop over el frames before starting over
const float LANTERN_COOLDOWN=2.0; // speed of lamas attack fel animation ha use it fel drawing  but logic is 1.5
const int WAVE_SIZES[3] = { 3, 5, 10 };//enemies per wave
const float SPAWN_INTERVAL = 10.0f;    // time between the enemies lama yetla3o