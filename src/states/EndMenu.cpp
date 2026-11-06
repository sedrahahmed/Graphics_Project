
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
   
    if (isClicked(restartButton)) changeState(PLAYING);
    if (isClicked(menuButton))    changeState(MAIN_MENU);
}

static void drawOutlinedText(const char* text, float x, float y, float scale, glm::vec4 color, float t){//t thickness in px
    for(float i=-t;i<=t;i+=t){//shift texte right left  3 times
        for(float j=-t;j<=t;j+=t){//bene3mel grid bet step by t and here shifts up down 3 times fa 9
            if (i == 0 && j == 0) continue;//skip center block
            drawText(text, x + i, y + j, scale, BLACK);

        }

    }
    drawText(text, x, y, scale, color);//xy da bottom left of my text
}

void drawEndMenu() {
    bool won = (getState() == WON);

    // See-through layer dark if we won dark red if we lost
    glm::vec4 overlay = won ? glm::vec4(0.0f, 0.0f, 0.0f, 0.6f) : glm::vec4(0.15f, 0.0f, 0.0f, 0.7f);
    drawRect(CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f, overlay);

   

    const char* title = won ? "THE CARNIVAL STAYS SEALED" : "THE TICKET IS LOST";
    const char* line  = won ? "You win!" : "You lose... the curse escapes.";
    drawOutlinedText(title, CENTER_X - textWidth(title, 1.0f) / 2.0f, 480, 1.0f, won ? TICKET_GOLD : glm::vec4(1.0f, 0.25f, 0.25f, 1.0f),1.5);//centter minus half toget left
    drawOutlinedText(line,  CENTER_X - textWidth(line, 0.6f) / 2.0f, 420, 0.6f, WHITE,1.5);

    drawButton(restartButton);
    drawButton(menuButton);
}
