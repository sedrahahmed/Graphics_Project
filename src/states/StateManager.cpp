// Decides which screen is showing and sends update/draw to that screen.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>      // only for key names like GLFW_KEY_ESCAPE
#include "StateManager.h"
#include "Input.h"
#include "Game.h"
#include "Cutscene.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "EndMenu.h"
#include "SoundMenu.h"
#include "Sound.h"
#include "HUD.h"

// the screen showing right now (the game opens on the main menu)
static GameState current = MAIN_MENU;

// lets other files ask which screen is showing
GameState getState() { return current; }

// Switches to another screen and does any setup that screen needs

void changeState(GameState next) {
    if (next == current) return;   // already on this screen
    GameState previous = current;  // remember where we came from
    current = next;

    switch (next) {
    case CUTSCENE:
        startCutscene(); 
        resetGame();  
        break;
    case PLAYING:
        if (previous != PAUSED) resetGame(); // from pause: keep the same round. from anywhere else: new round
        break;
    case WON:
        playSoundEffect(SOUND_WIN); // played once when the screen opens
        break;
    case LOST:
        playSoundEffect(SOUND_LOSE);
        break;
    default:
        break;   // main menu and sound menu and pause need no setup
    }
}

// logic of the screen that's showing (every frame)
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
        // Enter or Space skips the cutscene
        if (keyPressed(GLFW_KEY_ENTER) || keyPressed(GLFW_KEY_SPACE)) {
            skipCutscene();
            changeState(PLAYING);
        }
        else if (isCutsceneFinished()) changeState(PLAYING);   // cutscene ended
        break;

    case PLAYING:
        // Esc or pause button pauses, otherwise game runs
        if (keyPressed(GLFW_KEY_ESCAPE) || pauseIconClicked()) changeState(PAUSED);
        else updateGame(dt);
        break;

    case PAUSED:
        // updateGame is not called so game is frozen
        if (keyPressed(GLFW_KEY_ESCAPE)) changeState(PLAYING);   // Esc again = continue
        else updatePauseMenu();
        break;

    case WON:
    case LOST:
        updateEndMenu();   // game is frozen behind menu
        break;
    }
}

// Draws the screen that's showing (every frame)
// Whatever is drawn last appears on top
void drawCurrentState() {
    switch (current) {
    case MAIN_MENU:
        drawMainMenu();
        break;

    case SOUND_SETTINGS:
        //drawMainMenu();
        drawSoundMenu();
        break;

    case CUTSCENE:
        drawCutscene();
        break;

    case PLAYING:
        drawGame();
        break;

    case PAUSED:
        drawGame();        // frozen game first
        drawPauseMenu();   // then the menu on top
        break;

    case WON:
    case LOST:
        drawGame();        // frozen game first
        drawEndMenu();     // then the menu on top
        break;
    }
}
