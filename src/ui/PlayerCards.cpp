// PlayerCards.cpp. Owner: Person D
#include "PlayerCards.h"

static bool hasSelection = false;
static PlayerType selected = MIRROR_GUARD;

void updatePlayerCards() { }
void drawPlayerCards() { }

bool hasSelectedCard() { return hasSelection; }
PlayerType getSelectedCard() { return selected; }
void clearSelectedCard() { hasSelection = false; }
