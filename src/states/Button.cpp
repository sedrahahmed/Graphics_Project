#include "Button.h"
#include "Draw.h"
#include "Input.h"
#include "Text.h"
#include "Colors.h"
#include "Sound.h"

static const float BORDER = 3.0f; 
static const float TEXT_SCALE = 0.6f;
static const float TEXT_DROP = 10.0f;//bottom of text
static const float CORNER =6; //how big the stepped corners are
static const float DEPTH =6; //the height of the dark shadow strip at the bottom (the 3D look)
static const float LIGHT_H=4; //the height of the light strip at the top 

bool isHovered(const Button& b) {//check in el mouse 3al button
    return mouseInRect(b.x, b.y, b.w, b.h);
}

bool isClicked(const Button& b) {
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

    drawCutRect(b.x, b.y, b.w, b.h, CORNER, CURSE_RED); //outline
    drawCutRect(b.x, b.y, b.w - 2*BORDER, b.h - 2*BORDER, CORNER - 2, BTN_SHADOW); //shadow
    drawCutRect(b.x, b.y + DEPTH/2, b.w - 2*BORDER, b.h - 2*BORDER - DEPTH, CORNER - 2, face);//face
    drawRect(b.x,b.y+b.h/2-BORDER-LIGHT_H/2,b.w-2*BORDER-2*CORNER,LIGHT_H,0,BTN_LIGHT);
    float textX = b.x - textWidth(b.label, TEXT_SCALE) / 2.0f;
    float textY = b.y - TEXT_DROP + DEPTH/2;
    drawText(b.label, textX, textY, TEXT_SCALE, hover ? CURSE_RED : WHITE);
}
