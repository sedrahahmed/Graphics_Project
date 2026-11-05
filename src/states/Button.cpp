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
#include <cmath>

// ---------- Look of every button (change these to restyle all menus at once) ----------
static const float BORDER = 3.0f; 
static const float TEXT_SCALE = 0.6f;          // label size (1.0 = the font size given to initText)
static const float TEXT_DROP = 10.0f;          // drawText's y is the BOTTOM of the letters,
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

void drawButton(const Button& b) {
    bool hover = isHovered(b);

    // 1. Border: a gold rectangle a little bigger than the button, drawn first so it sits behind
    //drawRect(b.x, b.y, b.w + 2 * BORDER, b.h + 2 * BORDER, 0.0f, TICKET_GOLD);
    // 1. Border: 4 gold lines around the box (an outline, so the background shows through the box)
    float r=b.h/2;
    float midW=b.w-b.h;
    float midCirc= b.x-midW/2;
    float midCirc2= b.x+midW/2;
    const int SEG=20;
    glm::vec4 fill = hover ? glm::mix(BOARDWALK, WHITE, 0.3f) : BOARDWALK;
    fill.a=0.4;
    drawRect(b.x, b.y, midW, b.h, 0.0f, fill);
    for(int dx=0;dx<r;dx++){
        float sliceH = 2 * sqrt(r*r - dx*dx);
        drawRect(midCirc2 + dx + 0.5f, b.y, 1, sliceH, 0, fill);
        drawRect(midCirc - dx - 0.5f, b.y, 1, sliceH, 0, fill);
    }

    drawLine(midCirc,b.y+r,midCirc2,b.y+r,BORDER,TICKET_GOLD);
    drawLine(midCirc,b.y-r,midCirc2,b.y-r,BORDER,TICKET_GOLD);
    for(int i=0;i<SEG;i++){
        float angle1 = -90 + i * (180.0f / SEG);
        float angle2 = -90 + (i + 1) * (180.0f / SEG);
        float angle3 = 90 + i * (180.0f / SEG);
        float angle4 = 90 + (i + 1) * (180.0f / SEG);
        float x1 = midCirc2 + r * cos(glm::radians(angle1));
        float y1 = b.y + r * sin(glm::radians(angle1));
        float x2 = midCirc2 + r * cos(glm::radians(angle2));
        float y2 = b.y + r * sin(glm::radians(angle2));
        float x3 = midCirc + r * cos(glm::radians(angle3));
        float y3 = b.y + r * sin(glm::radians(angle3));
        float x4 = midCirc + r * cos(glm::radians(angle4));
        float y4 = b.y + r * sin(glm::radians(angle4));
        drawLine(x1, y1, x2, y2, BORDER, TICKET_GOLD); 
        drawLine(x3, y3, x4, y4, BORDER, TICKET_GOLD); 
      }

    

    

    // 3. The label, centered:
    //    textWidth tells us how wide the word is, so we start it half that width left of the center
    float textX = b.x - textWidth(b.label, TEXT_SCALE) / 2.0f;
    float textY = b.y - TEXT_DROP;
    drawText(b.label, textX, textY, TEXT_SCALE, hover ? TICKET_GOLD : WHITE);
}
