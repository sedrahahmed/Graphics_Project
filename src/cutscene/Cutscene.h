#pragma once
// Cutscene.h: the 10-20 second intro (guards and performers animated + dialogue).
// When it ends it calls changeState(PLAYING).
// Owner: Person D

void startCutscene();          // reset the timeline to 0
void updateCutscene(float dt);
void drawCutscene();
void skipCutscene();           // nice to have: Enter/Space skips
bool isCutsceneFinished();
