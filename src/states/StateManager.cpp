// StateManager.cpp. Owner: Person A
#include "StateManager.h"

static GameState current = MAIN_MENU;

GameState getState() { return current; }
void changeState(GameState next) { current = next; }   // TODO: setup for each state

void updateCurrentState(float dt) { }   // TODO: switch (current) { case PLAYING: updateGame(dt); ... }
void drawCurrentState() { }             // TODO: switch (current) { case PLAYING: drawGame(); ... }
