#include "Waves.h"
#include "Game.h"
#include <cstdlib>
#include "Config.h"
#include <ctime>

static int waveIndex = 0; // which wave is currently spawning (0..2)
static int spawnedCount = 0; // how many enemies have been spawned in this wave
static float spawnTimer = 0.0f; // time since last enemy spawned
static bool allDone = false; // true if all waves have been spawned

void resetWaves() { 
    waveIndex = 0;
    spawnedCount = 0;
    spawnTimer = 0.0f;
    allDone = false;
    srand((unsigned int)time(nullptr)); // seed random number generator
}
void updateWaves(float dt) {
    if (allDone) return; // all waves spawned
    if (spawnedCount< WAVE_SIZES[waveIndex]){
        spawnTimer += dt;
        if (spawnTimer >= SPAWN_INTERVAL) {
            spawnTimer = 0.0f;
            EnemyType type = (EnemyType)(rand() % (waveIndex + 1));
            int lane = rand() % LANE_COUNT; // random lane
            spawnEnemy(type, lane);
            spawnedCount++;
            
        }
    } else {
        if(waveIndex == 2)
            allDone = true; // all waves spawned
         else if(enemies.empty()) {
            waveIndex++;
            spawnedCount = 0;
            spawnTimer = 0.0f;
        }}
    }
 

int  getCurrentWave() { 
    return waveIndex + 1; // 1..3
 }
bool allWavesSpawned() { 
    return allDone;
 }
