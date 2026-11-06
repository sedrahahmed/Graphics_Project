#pragma once
// StateManager.h: which screen is showing.
enum GameState { MAIN_MENU, CUTSCENE, PLAYING, PAUSED, WON, LOST, SOUND_SETTINGS };

GameState getState();
void changeState(GameState next);

void updateCurrentState(float dt);  // called every frame by main
void drawCurrentState(); // PAUSED / LOST: draws game frozen + menu on top
