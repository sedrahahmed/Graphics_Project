// Economy.cpp. Owner: Person D
#include "Economy.h"
#include "Config.h"
#include "Game.h"
#include "Sound.h"
#include <cmath>

static int coins = START_COINS;
static const float PICKUP_RANGE = 30.0f; //The range within which the player can pick up a token. can change later

void resetEconomy() { coins = START_COINS; }
void updateEconomy(float dt) {
    for(Drop& d:drops){
        if(d.alive){
            for(Player& p:players){
                if(p.alive && std::abs(p.x - d.x) < PICKUP_RANGE && p.lane == d.lane) {
                    addCoins(COINS_PER_KILL);
                    playSoundEffect(SOUND_COIN);
                    d.alive = false;
                    break;
                }
            }
        }
    }
 }


int  getCoins() { return coins; }
void addCoins(int amount) {
    coins += amount;
    if (coins < 0) coins = 0;
 }
int  costOf(PlayerType type) { 
     switch (type) {
        case MIRROR_GUARD:   return COST_MELEE;
        case LANTERN_GUARD:  return COST_RANGED;
        case DEFENSE_GUARD:  return COST_DEFENSE;
        default:             return 0;
    }
}
bool canAfford(PlayerType type) { 
    return (getCoins() >= costOf(type)); }
bool spend(PlayerType type) { 
    if (canAfford(type)) {
        addCoins(-costOf(type));
        return true;
    }
    return false;
}
