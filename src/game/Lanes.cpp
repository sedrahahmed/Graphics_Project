#include "Lanes.h"
#include "Config.h"
#include "Game.h"
#include "Draw.h"
#include "Colors.h"
#include "Input.h"
#include "PlayerCards.h"
#include "Sound.h"
#include <GLFW/glfw3.h>

static bool laneSelected[LANE_COUNT] = { false }; //initalize all lanes to false
static int  hoveredLane = -1;
static int selectOrder[LANE_COUNT] = { 0 };
static int selectCounter = 0;

void resetLanes() {
    for (int i = 0; i < LANE_COUNT; i++) {
        laneSelected[i] = false;
        selectOrder[i]=0;
    }
    hoveredLane = -1;
    selectCounter=0;
}

void updateLanes() { 
   glm::vec2 m = mousePos(); //store mouse position in m

   if(m.x>=LANE_LEFT && m.x<LANE_RIGHT) //check en el mouse is between the left and right edges of the lanes
        hoveredLane = laneAtY(m.y); //we hover el lane
    else
        hoveredLane = -1;
   if(mouseClicked() && hoveredLane != -1) { //check en ehna f lane 
            laneSelected[hoveredLane] = !laneSelected[hoveredLane]; //toggle lane selection
            if(isLaneSelected(hoveredLane))
                selectOrder[hoveredLane]=++selectCounter;
            playSoundEffect(SOUND_LANE);
        
    }
    if (keyPressed(GLFW_KEY_W) || keyPressed(GLFW_KEY_UP)) { //If the W or Up key is pressed, we jump all selected lanes
                for (Player& p : players) {
                    if (p.alive && isLaneSelected(p.lane) && p.type != DEFENSE_JANA && p.state != JUMP && p.state != DUCK) { 
                        p.state = JUMP;
                        p.stateTimer = 0;
                    }
                }
            }
    if (keyPressed(GLFW_KEY_S) || keyPressed(GLFW_KEY_DOWN)) { //If the S or Down key is pressed, we duck all selected lanes
                for (Player& p : players) {
                    if (p.alive && isLaneSelected(p.lane) && p.type != DEFENSE_JANA && p.state != JUMP && p.state != DUCK) { 
                        p.state = DUCK;
                        p.stateTimer = 0;
                    }
                }
            }
        
}


void drawLanes() {
    float centerX = (LANE_LEFT + LANE_RIGHT) / 2;
    float width = LANE_RIGHT - LANE_LEFT;
    float height = LANE_HEIGHT - 6; //6 px smaller so the highlight doesn’t cover the rails
    for (int i=0; i<LANE_COUNT ; i++){
        float centerY = laneCenterY(i);
        if (laneSelected[i])//brnfr2 ben el hovered w el selected
            drawRect(centerX, centerY, width, height, 0, SELECTED_LANE);
        else if (hoveredLane == i)
            drawRect(centerX, centerY, width, height, 0, HIGHLIGHT);

    }
}

int  laneAtY(float y) { 
    float heightAboveLanes = y - LANE_BOTTOM; //Remove the empty space at the bottom
    if (heightAboveLanes < 0) 
        return -1;
    int lane = (int)(heightAboveLanes / LANE_HEIGHT);
    if (lane >= LANE_COUNT) 
        return -1;
    return lane;
}

bool isLaneSelected(int lane) { return lane >= 0 && lane < LANE_COUNT && laneSelected[lane]; }

int getLastSelectedLane(){
    int best=-1;
    for(int i=0;i<LANE_COUNT;i++){
        if(isLaneSelected(i)&&(best==-1||selectOrder[i]>selectOrder[best]))
             best=i;
    }
    return best;
}