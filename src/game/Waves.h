#pragma once
// Wave 1 -> 3 Magicians.
// Wave 2 -> 5 Magicians + Clowns. 
// Wave 3 -> 10, all three types.
// A wave ends when its last villain spawns the next wave waits until the field is clear.

void resetWaves();
void updateWaves(float dt);
int  getCurrentWave();       
bool allWavesSpawned();
