#pragma once

enum PlayerType { MIRROR_GUARD, LANTERN_GUARD, SENTINEL_GUARD };  // melee, ranged, defense
enum EnemyType { MAGICIAN, CLOWN, JESTER };                      // easy, medium, hard
enum AnimState { IDLE, WALK, ATTACK, JUMP, DUCK };
enum AttackHeight { HIGH, LOW, MELEE_HIT };   // HIGH: duck to dodge, LOW: jump to dodge

struct Player {
    PlayerType type;
    int   lane;          // 0..LANE_COUNT-1, never changes
    float x;             // horizontal position
    int   health;
    int   maxHealth;
    AnimState state;
    float stateTimer;    // how long it has been in this state (for animations)
    float attackTimer;   // counts down to the next attack
    bool  alive;
};

struct Enemy {
    EnemyType type;
    int   lane;
    float x;
    int   health;
    int   maxHealth;
    AnimState state;
    float stateTimer;
    float attackTimer;
    bool  alive;
};

struct Projectile {
    float x;
    int   lane;
    bool  fromPlayer;     // true: Lantern orb, false: card / juggling ball
    AttackHeight height;
    bool  hitting;        // playing its impact animation
    float hitTimer;
    bool  alive;
};

struct Drop {             // carnival token left behind by a defeated villain
    float x;
    int   lane;
    bool  alive;
};