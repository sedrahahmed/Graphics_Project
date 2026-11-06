#pragma once
// HUD.h: coin counter at the top (+ wave number, nice to have).
// Owner: Person D

void initHUD();   // loads the pause button picture, call once at startup
void drawHUD(int coins, int wave);

void drawPauseIcon();

bool pauseIconClicked();
