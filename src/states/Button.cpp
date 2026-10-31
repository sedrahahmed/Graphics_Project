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

// ---------- Look of every button (change these to restyle all menus at once) ----------
static const float BORDER = 4.0f;              // gold border thickness in pixels
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
    return isHovered(b) && mouseClicked();
}

void drawButton(const Button& b) {
    bool hover = isHovered(b);

    // 1. Border: a gold rectangle a little bigger than the button, drawn first so it sits behind
    drawRect(b.x, b.y, b.w + 2 * BORDER, b.h + 2 * BORDER, 0.0f, TICKET_GOLD);

    // 2. The box: boardwalk wood, mixed 30% toward white when hovered so it lights up
    //    glm::mix(a, b, t) = a when t = 0, b when t = 1, in between otherwise
    glm::vec4 fill = hover ? glm::mix(BOARDWALK, WHITE, 0.3f) : BOARDWALK;
    drawRect(b.x, b.y, b.w, b.h, 0.0f, fill);

    // 3. The label, centered:
    //    textWidth tells us how wide the word is, so we start it half that width left of the center
    float textX = b.x - textWidth(b.label, TEXT_SCALE) / 2.0f;
    float textY = b.y - TEXT_DROP;
    drawText(b.label, textX, textY, TEXT_SCALE, hover ? TICKET_GOLD : WHITE);
}
