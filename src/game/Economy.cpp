// Economy.cpp. Owner: Person D
#include "Economy.h"
#include "Config.h"

static int coins = START_COINS;

void resetEconomy() { coins = START_COINS; }
void updateEconomy(float dt) { }

int  getCoins() { return coins; }
void addCoins(int amount) { }
int  costOf(PlayerType type) { return 0; }
bool canAfford(PlayerType type) { return false; }
bool spend(PlayerType type) { return false; }
