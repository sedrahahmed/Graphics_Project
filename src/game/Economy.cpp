// Economy.cpp. Owner: Person D
#include "Economy.h"
#include "Config.h"

static int coins = START_COINS;

void resetEconomy() { coins = START_COINS; }
void updateEconomy(float dt) { }

int  getCoins() { return coins; }
void addCoins(int amount) {
    coins += amount;
    if (coins < 0) coins = 0;
 }
int  costOf(PlayerType type) { 
     switch (type) {
        case MIRROR_GUARD:   return COST_MELEE;
        case LANTERN_GUARD:  return COST_RANGED;
        case SENTINEL_GUARD: return COST_DEFENSE;
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
