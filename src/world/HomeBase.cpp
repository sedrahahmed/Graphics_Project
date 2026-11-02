// HomeBase.cpp. Owner: Person C
#include "HomeBase.h"
#include "Draw.h"
#include "Colors.h"
#include "Text.h"
#include <cmath>


void drawHomeBase(float time) { 
    drawRect(80,55,150,30,0,DARK_BROWN);
    drawRect(80,290,130,440,0,CURSE_RED);
    drawRect(18,290,6,440,0,TICKET_GOLD);
    drawRect(142,290,6,440,0,TICKET_GOLD);
    drawRect(80,330,96,116,0,TICKET_GOLD);
    drawRect(80,330,90,110,0,glm::vec4(0.08f, 0.04f, 0.10f, 1.0f));
    drawRect(80,268,110,10,0,BOARDWALK);
    for(int i=0; i<6; i++){
        float sx = 27 + i*21; //That spreads 6 stripes, each 21 wide, across the booth.
        glm::vec4 Stripecolor=(i%2==0) ? CURSE_RED : WHITE;
        drawRect(sx,405,21,34,0,Stripecolor);
        drawCircle(sx,388,10.5f,Stripecolor);

    }
    drawTriangle(80, 540, 160, 60, 0, CURSE_PURPLE);//roof
    drawLine(80,570,80,610,3,WHITE);//flag pole
    drawTriangle(92,602,22,14,-90,CURSE_RED); //flag
    
    drawRect(80,460,110,26,0,TICKET_GOLD);
    drawRect(80,460,104,20,0,DARK_BROWN);
    float scale = 0.35f;
    float tx = 80 - textWidth("TICKETS", scale) / 2.0f;
    drawText("TICKETS", tx, 452, scale, TICKET_GOLD);

    float glow = 0.5f + 0.5f *sin(time * 3.0f);
    float bob = sin(time * 2.0f) * 4.0f; //This makes it float up and down by 4 px.
    drawRect(80,330+bob, 76, 42, 0, glm::vec4(0.98f, 0.80f, 0.20f, 0.35f * glow));
    drawRect(80,330+bob, 60, 28, 0, TICKET_GOLD);
    drawCircle(50,330+bob,5, glm::vec4(0.08f, 0.04f, 0.10f, 1.0f));
    drawCircle(110,330+bob,5, glm::vec4(0.08f, 0.04f, 0.10f, 1.0f));


}