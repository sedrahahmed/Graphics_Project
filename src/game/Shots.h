#pragma once
// Shots.h: projectiles after they're fired: flying, dodging, hitting.
// Owner: Person B
//
// - Moves every projectile along its lane.
// - CURSED_CARD (HIGH) misses a ducking guard; JUGGLE_BALL (LOW) misses a jumping guard.
// - A guard behind a living Sentinel can't be hit: the Sentinel takes it instead.
// - On hit: lower health, start hit animation, then set alive = false.
// - Never removes the dead characters (Combat marks them, Game removes them).

void updateShots(float dt);
