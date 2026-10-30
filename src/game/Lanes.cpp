// Lanes.cpp. Owner: Person B
#include "Lanes.h"
#include "Config.h"

static bool laneSelected[LANE_COUNT] = { false };
static int  hoveredLane = -1;

void resetLanes() {
    for (int i = 0; i < LANE_COUNT; i++) laneSelected[i] = false;
    hoveredLane = -1;
}
void updateLanes() { }
void drawLanes() { }

int  laneAtY(float y) { return -1; }
int  getHoveredLane() { return hoveredLane; }
bool isLaneSelected(int lane) { return lane >= 0 && lane < LANE_COUNT && laneSelected[lane]; }
