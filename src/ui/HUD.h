#pragma once
//draws the top bar: coins, wave text and the pause icon.

void initHUD();   // loads the pause button picture, call once at startup
void drawHUD(int coins, int wave);
void drawPauseIcon();
bool pauseIconClicked();