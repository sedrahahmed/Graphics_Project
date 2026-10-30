// Cutscene.cpp. Owner: Person D
#include "Cutscene.h"

static float cutsceneTime = 0.0f;

void startCutscene() { cutsceneTime = 0.0f; }
void updateCutscene(float dt) { }
void drawCutscene() { }
void skipCutscene() { }
bool isCutsceneFinished() { return false; }
