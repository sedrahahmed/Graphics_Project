#pragma once

// Characters read this to draw and others changeit
enum PlayerType   { MELEE_SEIF, RANGED_LAMA, DEFENSE_JANA };  //el enums (o,1,2) they have numbers akeno array 
enum EnemyType    { MAGICIAN, CLOWN, JESTER };                        // easy, medium, hard
enum AnimState    { IDLE, WALK, ATTACK, JUMP, DUCK };//idle for jsna
enum AttackHeight { HIGH, LOW, MELEE_HIT };   // law hight duck to dodge we alw low jump to dodge wel  melee cant be dodged
enum ShotType     { LAMA_DART, MAGICIAN_HIT, JUGGLE_BALL };

struct Player {
    PlayerType type;
    int   lane;          // 0 (ta7t) to 3 bec  4 lanes we it never changes
    float x;             // middle of characters feet el x position
    int   health;//current
    int   maxHealth; //starting
    AnimState state;
    float stateTimer;    // how long it has been in this state (for animations)
    float attackTimer;   // counts down to the next attack i attack at 0 then reset bel cooldown
    bool  alive;    //3ashan amsa7o men el game law dead
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
    ShotType type;  // which picture to draw
    float x;//x pos
    int   lane;
    bool  fromPlayer; //law ah lama law no magician and clown
    AttackHeight height;
    bool  hitting;  //true when touches its target
    float hitTimer;//for drawing seconds since impact fa counts until hit duration
    bool  alive;//to remove fel draw
};

struct Drop {
    float x; //token which lane and if its collected (alive is false) or not yet     
    int   lane;
    bool  alive;
};
