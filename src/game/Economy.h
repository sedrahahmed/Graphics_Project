#pragma once
#include "Entities.h"

void resetEconomy();           
void updateEconomy();  //collect if player passes it

int  getCoins();
void addCoins(int amount);
int  costOf(PlayerType type);
bool canAfford(PlayerType type);    
bool spend(PlayerType type);        
