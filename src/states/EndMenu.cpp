// EndMenu.cpp. Owner: Person A
// The win OR lose screen. One file for both: getState() tells us which one we're in.
// Like the pause menu, it's drawn on top of the frozen game.
#include "EndMenu.h"
#include "StateManager.h"
#include "Button.h"
#include "Draw.h"
#include "Text.h"
#include "Colors.h"
#include "Config.h"

static const float CENTER_X = SCREEN_W / 2.0f;
static Button restartButton = { CENTER_X, 330, 280, 70, "RESTART" };
static Button menuButton    = { CENTER_X, 240, 280, 70, "MAIN MENU" };

void updateEndMenu() {
    // From WON / LOST, StateManager resets the round by itself when going to PLAYING
    if (isClicked(restartButton)) changeState(PLAYING);
    if (isClicked(menuButton))    changeState(MAIN_MENU);
}

void drawEndMenu() {
    bool won = (getState() == WON);

    // See-through layer: dark if we won, dark red if we lost
    glm::vec4 overlay = won ? glm::vec4(0.0f, 0.0f, 0.0f, 0.6f) : glm::vec4(0.3f, 0.0f, 0.0f, 0.6f);
    drawRect(CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, overlay);

    // Title + a story line, both centered
    const char* title = won ? "THE CARNIVAL STAYS SEALED" : "THE TICKET IS LOST";
    const char* line  = won ? "You win!" : "You lose... the curse escapes.";
    drawText(title, CENTER_X - textWidth(title, 1.0f) / 2.0f, 480, 1.0f, won ? TICKET_GOLD : CURSE_RED);
    drawText(line,  CENTER_X - textWidth(line, 0.6f) / 2.0f, 420, 0.6f, WHITE);

    drawButton(restartButton);
    drawButton(menuButton);
}
