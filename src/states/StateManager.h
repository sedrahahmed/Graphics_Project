#pragma once
// StateManager.h: which screen is showing.
// Owner: Person A. Shared contract: don't change without telling the team.

enum GameState { MAIN_MENU, CUTSCENE, PLAYING, PAUSED, WON, LOST, SOUND_SETTINGS };

GameState getState();
void changeState(GameState next);   // also handles setup, e.g. CUTSCENE -> startCutscene(),
                                    // restart -> resetGame()

void updateCurrentState(float dt);  // called every frame by main.cpp
void drawCurrentState();            // PAUSED / LOST: draws the game frozen + the menu on top
