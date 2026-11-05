// Button.cpp. Owner: Person A
// A button is just a rectangle + a word on it:
//   - hovered  = the mouse is inside the rectangle
//   - clicked  = hovered AND the left mouse button went down this frame
//   - drawn    = a gold border, a wooden box (lighter when hovered), and the label in the middle
//
// How a menu uses it:
//   Button start = { 640, 400, 260, 70, "START" };   // x, y (center), w, h, label
//   drawButton(start);                               // in drawMainMenu()
//   if (isClicked(start)) changeState(CUTSCENE);     // in updateMainMenu()
#include "Button.h"
#include "Draw.h"
#include "Input.h"
#include "Text.h"
#include "Colors.h"
#include "Sound.h"

// ---------- Look of every button (change these to restyle all menus at once) ----------
static const float BORDER = 3.0f; 
static const float TEXT_SCALE = 0.6f;          // label size (1.0 = the font size given to initText)
static const float TEXT_DROP = 10.0f;
static const float CORNER =6;
static const float DEPTH =6;
static const float LIGHT_H=4;   
                                               // so we move it down a bit to look centered

bool isHovered(const Button& b) {
    // mouseInRect already does the "is the mouse inside this box" check (x, y = center)
    return mouseInRect(b.x, b.y, b.w, b.h);
}

bool isClicked(const Button& b) {
    // mouseClicked() is only true on the ONE frame the button goes down,
    // so holding the mouse doesn't click the button over and over
    if(isHovered(b) && mouseClicked()){
        playSoundEffect(SOUND_CLICK);
        return true;
    }
    return false;
}

static void drawCutRect(float x,float y,float w,float h, float c,glm::vec4 col){
    drawRect(x, y, w, h - 2*c, 0, col);
    drawRect(x, y, w - 2*c, h, 0, col);
}
void drawButton(const Button& b) {
    bool hover = isHovered(b);
    glm::vec4 face = hover ? glm::mix(BTN_FACE, WHITE, 0.15f) : BTN_FACE;

    drawCutRect(b.x, b.y, b.w, b.h, CORNER, TICKET_GOLD); //outline
    drawCutRect(b.x, b.y, b.w - 2*BORDER, b.h - 2*BORDER, CORNER - 2, BTN_SHADOW); //shadow
    drawCutRect(b.x, b.y + DEPTH/2, b.w - 2*BORDER, b.h - 2*BORDER - DEPTH, CORNER - 2, face);//face
    drawRect(b.x,b.y+b.h/2-BORDER-LIGHT_H/2,b.w-2*BORDER-2*CORNER,LIGHT_H,0,BTN_LIGHT);
    float textX = b.x - textWidth(b.label, TEXT_SCALE) / 2.0f;
    float textY = b.y - TEXT_DROP + DEPTH/2;
    drawText(b.label, textX, textY, TEXT_SCALE, hover ? TICKET_GOLD : WHITE);
}
