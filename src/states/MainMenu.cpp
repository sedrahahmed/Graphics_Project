// MainMenu.cpp. Owner: Person A
// The first screen: title + Start, Sound, Exit.
// Every menu file has the same 2 parts:
//   update...(): check which button was clicked and switch screens
//   draw...():   draw the background, title and buttons
#include "MainMenu.h"
#include "StateManager.h"
#include "Button.h"
#include "Draw.h"
#include "Text.h"
#include "Input.h"
#include "Colors.h"
#include "Config.h"

// The buttons: x, y (center), width, height, label.
// They're "static" so they're made once and shared by update and draw.
static const float CENTER_X = SCREEN_W / 2.0f;
static Button startButton = { CENTER_X, 380, 280, 70, "START" };
static Button soundButton = { CENTER_X, 290, 280, 70, "SOUND" };
static Button exitButton  = { CENTER_X, 200, 280, 70, "EXIT" };

void updateMainMenu() {
    if (isClicked(startButton)) changeState(CUTSCENE);        // cutscene, then the game
    if (isClicked(soundButton)) changeState(SOUND_SETTINGS);  // volume screen
    if (isClicked(exitButton))  requestExit();                // closes the window
}

void drawMainMenu() {
    // Background: fill the whole screen with the night sky
    drawRect(CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, NIGHT_SKY);

    // Title, centered: start half its width to the left of the center
    const char* title = "CARNIVAL OF THE CURSED";
    float scale = 1.2f;
    drawText(title, CENTER_X - textWidth(title, scale) / 2.0f, 540, scale, TICKET_GOLD);

    drawButton(startButton);
    drawButton(soundButton);
    drawButton(exitButton);
}
