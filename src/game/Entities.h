#pragma once
// Entities.h: what every character and object IS, as data.
// Logic changes these values; Characters reads them to draw.
// Shared contract: don't change without telling the team.

enum PlayerType   { MIRROR_GUARD, LANTERN_GUARD, SENTINEL_GUARD };   // melee, ranged, defense
enum EnemyType    { MAGICIAN, CLOWN, JESTER };                        // easy, medium, hard
enum AnimState    { IDLE, WALK, ATTACK, JUMP, DUCK };
enum AttackHeight { HIGH, LOW, MELEE_HIT };   // HIGH: duck to dodge, LOW: jump to dodge
enum ShotType     { LANTERN_ORB, CURSED_CARD, JUGGLE_BALL };

struct Player {
    PlayerType type;
    int   lane;          // 0..LANE_COUNT-1, never changes
    float x;             // horizontal position (center of the feet)
    int   health;
    int   maxHealth;
    AnimState state;
    float stateTimer;    // how long it has been in this state (for animations)
    float attackTimer;   // counts down to the next attack
    bool  alive;         // set to false to remove it (Game removes it at end of frame)
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
    ShotType type;        // which picture to draw
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
