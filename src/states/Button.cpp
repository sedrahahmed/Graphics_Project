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
static const float FILL_OPAQUENESS=0.3;             // gold border thickness in pixels
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
    float left = b.x - b.w / 2, right = b.x + b.w / 2;
    float top  = b.y + b.h / 2, bottom = b.y - b.h / 2;
    drawLine(left,  top,    right, top,    BORDER, TICKET_GOLD);   // top
    drawLine(left,  bottom, right, bottom, BORDER, TICKET_GOLD);   // bottom
    drawLine(left,  bottom, left,  top,    BORDER, TICKET_GOLD);   // left
    drawLine(right, bottom, right, top,    BORDER, TICKET_GOLD);   // right

    // 2. The box: boardwalk wood, mixed 30% toward white when hovered so it lights up
    //    glm::mix(a, b, t) = a when t = 0, b when t = 1, in between otherwise
    glm::vec4 fill = hover ? glm::mix(BOARDWALK, WHITE, 0.3f) : BOARDWALK;
   fill.a=0.4;//4th beta3 el heya el alpha ben change its value
    drawRect(b.x, b.y, b.w, b.h, 0.0f, fill);

    // 3. The label, centered:
    //    textWidth tells us how wide the word is, so we start it half that width left of the center
    float textX = b.x - textWidth(b.label, TEXT_SCALE) / 2.0f;
    float textY = b.y - TEXT_DROP;
    drawText(b.label, textX, textY, TEXT_SCALE, hover ? TICKET_GOLD : WHITE);
}
