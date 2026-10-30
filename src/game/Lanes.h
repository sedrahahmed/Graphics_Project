#pragma once
// Lanes.h: lane hover, click-to-select (several at once), jump/duck, drawing the lanes.
// Owner: Person B
//
// Click rule: if a card is selected -> clicking a lane places that guard there.
//             otherwise            -> clicking a lane toggles its selection.

void resetLanes();             // nothing selected
void updateLanes();            // hover, clicks, W/Up = jump, S/Down = duck (selected lanes)
void drawLanes();              // boardwalk lanes + hover/selected highlight

int  laneAtY(float y);         // which lane is at this height (-1 if none)
int  getHoveredLane();         // -1 if the mouse isn't over a lane
bool isLaneSelected(int lane);
