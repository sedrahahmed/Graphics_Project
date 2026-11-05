// Combat.cpp. Owner: Person C
#include "Combat.h"
#include "Game.h"
#include "Config.h"
#include "Sound.h"

static const float MELEE_RANGE = 50.0f; //fix later if not alligned with the art

//Helpers
static int enemyInFront(const Player& p){
    int closest = -1;
    for(int i=0; i<(int)enemies.size(); i++){
        const Enemy& enemy = enemies[i];
        float distance = enemy.x - p.x;
        if(enemy.alive && enemy.lane == p.lane && enemy.x >= p.x && distance <= MELEE_RANGE) {
            if(closest == -1 || enemy.x < enemies[closest].x) {
                closest = i;
            }
        }
    }
    return closest;
}

static int guardInFront(const Enemy& e){
    int closest = -1;
    for(int i=0; i<(int)players.size(); i++){
        const Player& player = players[i];
        float distance = e.x - player.x;                
        if(player.alive && player.lane == e.lane && player.x<=e.x && distance <= MELEE_RANGE) {  //If the player is alive, in the same lane, in front of the enemy(x is smaller 3ashan player on the left), and within melee range
            if(closest == -1 || player.x > players[closest].x) {   
                closest = i;
            }
        }
    }
    return closest;
}


void updateCombat(float dt) {
    for(Player &p: players){   
        if(p.alive && p.health <= 0) {
            p.alive = false;
            playSoundEffect(SOUND_DEATH);
        }
    }
    for(Enemy &e: enemies){
        if(e.alive && e.health <= 0) {
            e.alive = false;
            playSoundEffect(SOUND_DEATH);
            dropToken(e.lane, e.x); //Drop a token when an enemy dies
            
        }
    }
    for(Player &p: players){
        if(p.alive) {
            int enemyIndex = enemyInFront(p);
            if(enemyIndex != -1 && (p.state == WALK || p.state == IDLE)) {   //theres an enemy infront and the player is walking and not a defense guard
                p.state = ATTACK;
                p.stateTimer = 0.0f;
            }
            if(enemyIndex == -1 && p.state == ATTACK) {  //theres no enemy infront and i was attacking but now theres no enemy infront so go back to walking
                if(p.type==DEFENSE_GUARD)
                    p.state=IDLE;
                else
                    p.state=WALK;
                p.stateTimer = 0.0f;
            }
            p.attackTimer -= dt;            
            if(p.attackTimer<=0){
                //attack logic
                switch (p.type){
                    case MIRROR_GUARD:{
                        if(enemyIndex != -1) {
                            Enemy& e = enemies[enemyIndex];
                            e.health -= DMG_PLAYER;
                            playSoundEffect(SOUND_MELEE_ATTACK);
                        }
                        break;
                    }
                    case LANTERN_GUARD:{
                        fireShot(LANTERN_ORB, p.lane, p.x + 40.0f, true); //fire a lantern orb from the guard's position
                        playSoundEffect(SOUND_RANGED_ATTACK);
                        break;
                    }
                    case DEFENSE_GUARD:{
                        break;
                    }
                }
                p.attackTimer = (p.type == LANTERN_GUARD) ? LANTERN_COOLDOWN : ATTACK_COOLDOWN;
            }
        }
    }
    for(Enemy &e: enemies){
        if(e.alive) {
            int playerIndex = guardInFront(e);
            if(playerIndex != -1 && e.state == WALK) {   //theres a player infront and the enemy is walking
                e.state = ATTACK;
                e.stateTimer = 0.0f;
            }
            if(playerIndex == -1 && e.state == ATTACK) {  //theres no player infront and i was attacking but now theres no player infront so go back to walking
                e.state = WALK;
                e.stateTimer = 0.0f;
            }
            e.attackTimer -= dt;
            if(e.attackTimer<=0){
                //attack logic
                switch(e.type){
                    case MAGICIAN:{
                        fireShot(CURSED_CARD, e.lane, e.x - 40.0f, false); //fire a cursed card from the enemy's position
                        playSoundEffect(SOUND_RANGED_ATTACK);
                        break;
                    }
                    case CLOWN:{
                        fireShot(JUGGLE_BALL, e.lane, e.x - 40.0f, false); //fire a juggle ball from the enemy's position
                        playSoundEffect(SOUND_ROLLING_BALL);
                        break;
                    } 
                    case JESTER:{
                        if(playerIndex != -1) {
                            Player& p = players[playerIndex];
                            p.health -= DMG_JESTER;
                            playSoundEffect(SOUND_MELEE_ATTACK);
                        }
                        break;
                    }
                }
                e.attackTimer = ATTACK_COOLDOWN;
            }

        }
    }
}
