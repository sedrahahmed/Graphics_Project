// Lanes.cpp. Owner: Person B
#include "Lanes.h"
#include "Config.h"
#include "Game.h"
#include "Draw.h"
#include "Colors.h"
#include "Input.h"
#include "PlayerCards.h"
#include <GLFW/glfw3.h>

static bool laneSelected[LANE_COUNT] = { false };
static int  hoveredLane = -1;

void resetLanes() {
    for (int i = 0; i < LANE_COUNT; i++) laneSelected[i] = false;
    hoveredLane = -1;
}
void updateLanes() { 
   glm::vec2 m = mousePos(); //Get the mouse position and store it in a variable called m.
   if(m.x>=LANE_LEFT && m.x<LANE_RIGHT) //If the mouse is between the left and right edges of the lanes, we check which lane it is over. If it is not over any lane, we set hoveredLane to -1.
        hoveredLane = laneAtY(m.y);
    else
        hoveredLane = -1;
   if(mouseClicked() && hoveredLane != -1) { //If the mouse was clicked and the mouse is over a lane, then we check if a card is selected. If it is, we place the guard in that lane. If not, we toggle the selection of that lane.
        if (hasSelectedCard()) { 
            if(placeGuard(getSelectedCard(), hoveredLane)) {
                clearSelectedCard(); //Clear the selected card after placing the guard.
            }
        } else {
            laneSelected[hoveredLane] = !laneSelected[hoveredLane];
        }
    }
    if (keyPressed(GLFW_KEY_W) || keyPressed(GLFW_KEY_UP)) { //If the W or Up key is pressed, we jump all selected lanes.
                for (Player& p : players) {
                    if (p.alive && isLaneSelected(p.lane) && p.type != SENTINEL_GUARD && p.state != JUMP && p.state != DUCK) { 
                        p.state = JUMP;
                        p.stateTimer = 0;
                    }
                }
            }
    if (keyPressed(GLFW_KEY_S) || keyPressed(GLFW_KEY_DOWN)) { //If the S or Down key is pressed, we duck all selected lanes.
                for (Player& p : players) {
                    if (p.alive && isLaneSelected(p.lane) && p.type != SENTINEL_GUARD && p.state != JUMP && p.state != DUCK) { 
                        p.state = DUCK;
                        p.stateTimer = 0;
                    }
                }
            }
        
}


void drawLanes() {//Draws 4 wooden boardwalk lanes. A lane the mouse is over gets a light overlay, and a selected lane gets a stronger one. 
    float centerX = (LANE_LEFT + LANE_RIGHT) / 2;
    float width = LANE_RIGHT - LANE_LEFT;
    float height = LANE_HEIGHT - 6;
    for (int i=0; i<LANE_COUNT ; i++){
        float centerY = laneCenterY(i);
        drawRect(centerX, centerY, width, height, 0, BOARDWALK);
    
        for (float j=LANE_LEFT; j<LANE_RIGHT; j+= 40){
            drawLine(j, centerY - height / 2, j, centerY + height / 2, 2, DARK_BROWN);
    }
        if (laneSelected[i])
            drawRect(centerX, centerY, width, height, 0, SELECTED_LANE);
        else if (hoveredLane == i)
            drawRect(centerX, centerY, width, height, 0, HIGHLIGHT);

}

}

int  laneAtY(float y) { 
    float heightAboveLanes = y - LANE_BOTTOM; //Remove the empty space at the bottom. Make a new float variable that equals y minus LANE_BOTTOM. Now the bottom of lane 0 counts as 0.
    if (heightAboveLanes < 0) 
        return -1;
    int lane = (int)(heightAboveLanes / LANE_HEIGHT); //Divide by the lane height to get how many lanes up it is. Store the result in an int variable. Converting a float to an int drops the decimal part, so 2.7 becomes 2. The way to write that is (int)(something).
    if (lane >= LANE_COUNT) 
        return -1;
    return lane;
}
int  getHoveredLane() { return hoveredLane; }
bool isLaneSelected(int lane) { return lane >= 0 && lane < LANE_COUNT && laneSelected[lane]; }
