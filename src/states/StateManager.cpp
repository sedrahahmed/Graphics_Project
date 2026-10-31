// StateManager.cpp. Owner: Person A
// Decides which screen is showing, and sends update/draw to that screen.
//
//   MAIN_MENU --Start--> CUTSCENE --ends--> PLAYING --Esc--> PAUSED
//   PLAYING --win/lose--> WON / LOST
//   MAIN_MENU <--> SOUND_SETTINGS
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>      // only for the key codes (GLFW_KEY_ESCAPE, ...)
#include "StateManager.h"
#include "Input.h"
#include "Game.h"
#include "Cutscene.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "EndMenu.h"
#include "SoundMenu.h"

static GameState current = MAIN_MENU;

GameState getState() { return current; }

// Runs the setup a screen needs when we enter it.
// How the menus use it:
//   Main menu Start     -> changeState(CUTSCENE)
//   Pause Continue      -> changeState(PLAYING)                 (keeps the round going)
//   Pause Restart       -> resetGame(); changeState(PLAYING);   (fresh round)
//   Win/Lose Restart    -> changeState(PLAYING)                 (resets automatically)
//   Any "Main Menu"     -> changeState(MAIN_MENU)
void changeState(GameState next) {
    if (next == current) return;
    GameState previous = current;
    current = next;

    switch (next) {
    case CUTSCENE:
        startCutscene();
        break;
    case PLAYING:
        // Coming back from pause continues the same round. Anything else starts a new one.
        if (previous != PAUSED) resetGame();
        break;
    default:
        break;   // menus, PAUSED, WON, LOST need no setup
    }
}

void updateCurrentState(float dt) {
    switch (current) {
    case MAIN_MENU:
        updateMainMenu();
        break;

    case SOUND_SETTINGS:
        updateSoundMenu();
        break;

    case CUTSCENE:
        updateCutscene(dt);
        if (keyPressed(GLFW_KEY_ENTER) || keyPressed(GLFW_KEY_SPACE)) skipCutscene();
        if (isCutsceneFinished()) changeState(PLAYING);
        break;

    case PLAYING:
        if (keyPressed(GLFW_KEY_ESCAPE)) changeState(PAUSED);
        else updateGame(dt);
        break;

    case PAUSED:
        // The game is NOT updated here, so everything stays frozen
        if (keyPressed(GLFW_KEY_ESCAPE)) changeState(PLAYING);   // Esc again = continue
        else updatePauseMenu();
        break;

    case WON:
    case LOST:
        updateEndMenu();   // the field stays frozen behind the menu
        break;
    }
}

void drawCurrentState() {
    switch (current) {
    case MAIN_MENU:
        drawMainMenu();
        break;

    case SOUND_SETTINGS:
        drawSoundMenu();
        break;

    case CUTSCENE:
        drawCutscene();
        break;

    case PLAYING:
        drawGame();
        break;

    case PAUSED:
        drawGame();        // frozen field underneath
        drawPauseMenu();   // menu on top
        break;

    case WON:
    case LOST:
        drawGame();
        drawEndMenu();
        break;
    }
}
