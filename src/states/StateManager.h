#pragma once

enum GameState { MAIN_MENU, CUTSCENE, PLAYING, PAUSED, WON, LOST, SOUND_SETTINGS };

GameState getState();
void changeState(GameState next);   // also handles setup, e.g. PLAYING after restart → resetGame()

void updateCurrentState(float dt);  // called every frame by main.cpp
void drawCurrentState();