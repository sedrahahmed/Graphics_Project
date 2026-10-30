#pragma once

// ---------- Screen layout (pixels, origin bottom-left) ----------
const int   SCREEN_W = 1280;
const int   SCREEN_H = 720;

const int   LANE_COUNT = 4;
const float LANE_BOTTOM = 40.0f;      // y where the lowest lane starts
const float LANE_HEIGHT = 130.0f;     // each lane is 130 px tall
const float LANE_LEFT = 160.0f;     // lanes start right of the booth
const float LANE_RIGHT = 1280.0f;    // enemies enter here
// Lane 0 is the bottom lane. Center of lane i:
//   LANE_BOTTOM + LANE_HEIGHT * i + LANE_HEIGHT / 2

const float HOME_BASE_X = 80.0f;      // center of the ticket booth
const float HUD_Y = 600.0f;     // cards + coin counter live above this

// ---------- Economy ----------
const int START_COINS = 30;
const int COST_MELEE = 10;         // Mirror Guard
const int COST_RANGED = 20;         // Lantern Guard (2x)
const int COST_DEFENSE = 30;         // Sentinel Guard (3x)
const int COINS_PER_KILL = 10;

// ---------- Health (from the brief) ----------
const int HP_MELEE = 5, HP_RANGED = 5, HP_DEFENSE = 7;
const int HP_MAGICIAN = 5, HP_CLOWN = 5, HP_JESTER = 7;

// ---------- Damage (from the brief) ----------
const int DMG_PLAYER = 1;
const int DMG_ENEMY = 1;
const int DMG_JESTER = 2;

// ---------- Speeds (pixels per second) ----------
const float SPEED_GUARD = 40.0f;
const float SPEED_ENEMY = 30.0f;
const float SPEED_JESTER = 70.0f;   // "runs"
const float SPEED_PROJECTILE = 250.0f;

// ---------- Timing (seconds) ----------
const float ATTACK_COOLDOWN = 1.0f;    // time between attacks
const float JUMP_DURATION = 0.6f;
const float DUCK_DURATION = 0.6f;

// ---------- Waves (from the brief) ----------
const int WAVE_SIZES[3] = { 3, 5, 10 };
const float SPAWN_INTERVAL = 4.0f;    // seconds between enemy spawns