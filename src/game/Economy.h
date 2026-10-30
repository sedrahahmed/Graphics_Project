#pragma once
// Economy.h: coins, costs, token pickup, and which cards are unlocked.
// Owner: Person D

#include "Entities.h"

void resetEconomy();            // coins = START_COINS
void updateEconomy(float dt);   // a guard touching a token -> collect it (alive = false, add coins)

int  getCoins();
void addCoins(int amount);
int  costOf(PlayerType type);
bool canAfford(PlayerType type);    // also = "is this card unlocked?"
bool spend(PlayerType type);        // pays if possible; returns false if not enough coins
