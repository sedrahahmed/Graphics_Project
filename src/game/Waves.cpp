#include "Waves.h"
#include "Game.h"
#include <cstdlib>
#include "Config.h"
#include <ctime>

static int waveIndex = 0;
static int spawnedCount = 0; // no. of villains spawned in this wave
static float spawnTimer = 0.0f; // time since last villain spawned
static bool allDone = false; // true if all waves have been spawned

void resetWaves() { 
    waveIndex = 0;
    spawnedCount = 0;
        spawnTimer = SPAWN_INTERVAL - FIRST_SPAWN_DELAY; //first enemy comes quicker
    allDone = false;
    srand((unsigned int)time(nullptr)); // seed el random number generator b el current time 3ashan maykonsh nafs el lanes kol new game
}
void updateWaves(float dt) {
    if (allDone) return; 
    if (spawnedCount< WAVE_SIZES[waveIndex]){ //check law lesa feh makan fel lane nehot villians
        spawnTimer += dt;
        if (spawnTimer >= SPAWN_INTERVAL) { //check en el wa2t khls
            spawnTimer = 0.0f;
            EnemyType type = (EnemyType)(rand() % (waveIndex + 1));//check anhy wave 3ashan neshof hanazel anhy villian/s
            int lane = rand() % LANE_COUNT; // betkhtar random lane t-spwan el villian feh
            spawnEnemy(type, lane);
            spawnedCount++;
            
        }
    } else {//kol el villains nezlo
        if(waveIndex == 2)
            allDone = true; 
         else if(enemies.empty()) {// not last wave
            waveIndex++;
            spawnedCount = 0;
            spawnTimer = SPAWN_INTERVAL - FIRST_SPAWN_DELAY; //new wave first enemy after 3 sec
        }}
    }
 

int  getCurrentWave() { 
    return waveIndex + 1; 
 }
bool allWavesSpawned() { 
    return allDone;
 }
