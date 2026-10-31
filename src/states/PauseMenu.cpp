// PauseMenu.cpp. Owner: Person A
// Opened with Esc during the game. StateManager draws the frozen game FIRST,
// then this menu on top, so the game shows through the dark see-through layer.
#include "PauseMenu.h"
#include "StateManager.h"
#include "Game.h"          // resetGame()
#include "Button.h"
#include "Draw.h"
#include "Text.h"
#include "Colors.h"
#include "Config.h"

static const float CENTER_X = SCREEN_W / 2.0f;
static Button continueButton = { CENTER_X, 400, 280, 70, "CONTINUE" };
static Button restartButton  = { CENTER_X, 310, 280, 70, "RESTART" };
static Button menuButton     = { CENTER_X, 220, 280, 70, "MAIN MENU" };

void updatePauseMenu() {
    // Continue: back to the SAME round (StateManager doesn't reset when coming from PAUSED)
    if (isClicked(continueButton)) changeState(PLAYING);

    // Restart: wipe the round first, then go back to playing
    if (isClicked(restartButton)) {
        resetGame();
        changeState(PLAYING);
    }

    if (isClicked(menuButton)) changeState(MAIN_MENU);
}

void drawPauseMenu() {
    // Dark see-through layer over the frozen game (alpha 0.6 = 60% dark)
    drawRect(CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.6f));

    const char* title = "PAUSED";
    float scale = 1.2f;
    drawText(title, CENTER_X - textWidth(title, scale) / 2.0f, 500, scale, WHITE);

    drawButton(continueButton);
    drawButton(restartButton);
    drawButton(menuButton);
}
