
#include "Economy.h"
#include "Config.h"
#include "Game.h"
#include "Sound.h"
#include <cmath>//needed it for the abs function
//coin logic
static int coins = START_COINS;
static const float PICKUP_RANGE = 30.0f; //The range within which the player can pick up a token. can change later

void resetEconomy() { coins = START_COINS; }//called when we restart
void updateEconomy() {//once per frame han call it
    for(Drop& d:drops){
        if(d.alive){//token mawgooda
            for(Player& p:players){
                if(p.alive && std::abs(p.x - d.x) < PICKUP_RANGE && p.lane == d.lane) {
                    addCoins(COINS_PER_KILL);
                    playSoundEffect(SOUND_COIN);
                    d.alive = false;//token was taken bas still drawn remove dead hatemsa7ha
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
        case MELEE_SEIF:   return COST_MELEE;
        case RANGED_LAMA:  return COST_RANGED;
        case DEFENSE_JANA:  return COST_DEFENSE;
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
