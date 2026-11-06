
#include "Combat.h"
#include "Game.h"
#include "Config.h"
#include "Sound.h"
//runs mara every frame men update game
//checks whos dead and makes them dead bas doesnt remove and lets players find a target 
static const float MELEE_RANGE = 50.0f; //max distance for melee characters to be considered touching fa hay attack

//Helpers
static int enemyInFront(const Player& p){
    int closest = -1;//if fighting no one
    for(int i=0; i<(int)enemies.size(); i++){
        const Enemy& enemy = enemies[i];
        float distance = enemy.x - p.x;
        if(enemy.alive && enemy.lane == p.lane && enemy.x >= p.x && distance <= MELEE_RANGE) {
            if(closest == -1 || enemy.x < enemies[closest].x) {//if first enemy we meet or if its closer than closest so far
                closest = i;
            }
        }
    }
    return closest;
}
//exact same as above bas for players
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
    for(Player &p: players){   //checking if dead and making them dead if yes
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
                p.state = ATTACK;// also makes them stop moving (state attack is not moving)
                p.stateTimer = 0.0f;//reset to make animations start from beginning frame 0 
            }
            if(enemyIndex == -1 && p.state == ATTACK) {  //theres no enemy infront and i was attacking but now theres no enemy infront so go back to walking
                if(p.type==DEFENSE_JANA)
                    p.state=IDLE;
                else
                    p.state=WALK;
                p.stateTimer = 0.0f;
            }
            p.attackTimer -= dt;            //constantly runs counts cooldown
            if(p.attackTimer<=0){ //time to attack
                //attack logic
                switch (p.type){
                    case MELEE_SEIF:{
                        if(enemyIndex != -1) {
                            Enemy& e = enemies[enemyIndex];
                            e.health -= DMG_PLAYER;
                            playSoundEffect(SOUND_MELEE_ATTACK);
                        }
                        break;
                    }
                    case RANGED_LAMA:{
                        fireShot(LAMA_DART, p.lane, p.x + 40.0f, true); //fire a lantern orb from the guard's position +40 px
                        playSoundEffect(SOUND_RANGED_ATTACK);
                        break;
                    }
                    case DEFENSE_JANA:{
                        break; //doesnt attack only blocks
                    }
                }
                p.attackTimer = (p.type == RANGED_LAMA) ? LANTERN_COOLDOWN : HERO_ATTACK_COOLDOWN;
//reset the attack timer to the cooldown time(wait 1.5 seconds before attacking again) in lamas case 2
            }
        }
    }//same as the players but for villains now
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
                        fireShot(MAGICIAN_HIT, e.lane, e.x - 40.0f, false); //fire a cursed card from the enemy's position. false bec its not from the player its enemy
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
                e.attackTimer = ENEMY_ATTACK_COOLDOWN;//reset the attack timer to the cooldown time(wait 4 seconds before attacking again)
            }

        }
    }
}
