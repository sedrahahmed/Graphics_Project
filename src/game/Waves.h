#pragma once
// Waves.h: when and where villains appear.
// Owner: Person D
//
// Wave 1: 3 Magicians. Wave 2: 5, Magicians + Clowns. Wave 3: 10, all three types.
// A wave ends when its last villain spawns; the next wave waits until the field is clear.

void resetWaves();
void updateWaves(float dt);    // may call spawnEnemy(type, randomLane)

int  getCurrentWave();         // 1..3 (for the HUD)
bool allWavesSpawned();        // true after wave 3's last villain spawned
                               // win = allWavesSpawned() && no enemies left
