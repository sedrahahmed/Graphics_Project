#pragma once
// Combat.h: guards and villains meeting, attack timers, melee damage, deaths.
// Owner: Person C
//
// - Same lane + touching -> both stop and attack.
// - Melee (Mirror Guard, Jester): damage directly.
// - Ranged (Lantern Guard, Magician, Clown): call fireShot(...); Shots does the rest.
// - Health <= 0 -> alive = false. A dead villain -> dropToken(...).

void updateCombat(float dt);
