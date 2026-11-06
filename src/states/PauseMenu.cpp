// Opened with Esc during the game
// StateManager leaves the frozen game then pause menu on top, so the game shows through pause menu

#include "PauseMenu.h"
#include "StateManager.h"
#include "Game.h"          // resetGame()
#include "Button.h"
#include "Draw.h"
#include "Text.h"
#include "Colors.h"
#include "Config.h"

static const float CENTER_X = SCREEN_W / 2.0f; // get center of the screen 
static Button continueButton = {CENTER_X, 400, 280, 70, "CONTINUE"};
static Button restartButton = {CENTER_X, 310, 280, 70, "RESTART"};
static Button menuButton = {CENTER_X, 220, 280, 70, "MAIN MENU"};

void updatePauseMenu() {
    if (isClicked(continueButton)) changeState(PLAYING); // StateManager doesn't reset after pause
    
    if (isClicked(restartButton)) {
        resetGame();
        changeState(PLAYING);
    }
    if (isClicked(menuButton)) changeState(MAIN_MENU);
}

void drawPauseMenu() {
    // rotation 0.0f, colour(r,g,b,opaqueness) 
    drawRect(CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.6f)); // Dark transparent rectangle layer over frozen game
    
    const char* title = "PAUSED";
    float scale = 1.2f;
    
    // Draw the title text centered on screen at y = 500
    drawText(title, CENTER_X - textWidth(title, scale) / 2.0f, 500, scale, WHITE);

    drawButton(continueButton);
    drawButton(restartButton);
    drawButton(menuButton);
}
