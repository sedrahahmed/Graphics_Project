
#include "MainMenu.h"
#include "StateManager.h"
#include "Button.h"
#include "Draw.h"
#include "Text.h"
#include "Input.h"
#include "Colors.h"
#include "Config.h"

static const float CENTER_X = SCREEN_W / 2.0f;
static Button startButton = { CENTER_X, 380, 200, 70, "START" };//x y width height label
static Button soundButton = { CENTER_X, 290, 200, 70, "SOUND" };
static Button exitButton  = { CENTER_X, 200, 200, 70, "EXIT" };
static unsigned int menuTexture=0;//id of pic bg

void initMainMenu(){
    
    menuTexture=loadTexture(ASSET_DIR"background/Main_menu_image.png");
}
unsigned int getMenuTexture() { return menuTexture; }

void updateMainMenu() {
    if (isClicked(startButton)) changeState(CUTSCENE);   // cutscene then game
    if (isClicked(soundButton)) changeState(SOUND_SETTINGS); // volume screen
    if (isClicked(exitButton))  requestExit();// closes the window
}

void drawMainMenu() {
    
    drawRect(CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, NIGHT_SKY);//incase pic mesh bet load bas
    drawSprite(menuTexture, 0, 1, CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, WHITE);//pic above the bg colour we white means unchanged colour
  
    drawButton(startButton);
    drawButton(soundButton);
    drawButton(exitButton);
}
