#pragma once
// PlayerCards.h: the 3 guard cards.
// Locked (can't afford): grayscale, no hover, no click.
// Unlocked: normal colors, hover effect, click to select (click again to unselect).
// Owner: Person D

#include "Entities.h"

void updatePlayerCards();      // hover + click; clears the selection if that card re-locks
void drawPlayerCards();        // selected card gets a glowing border

bool hasSelectedCard();        // is a card currently picked?
PlayerType getSelectedCard();  // which one
void clearSelectedCard();      // unpick it (after placing, or right-click to cancel)
