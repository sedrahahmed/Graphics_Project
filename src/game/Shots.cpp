// Shots.cpp. Owner: Person B
#include "Shots.h"
#include "Game.h"
#include <cmath>

static const float HIT_RANGE = 35;

void updateShots(float dt) {
    for(Projectile& p:shots){
        if(p.alive){
            float speed;
            switch (p.type) {
                case LANTERN_ORB: speed = SPEED_DART;  break;
                case CURSED_CARD: speed = SPEED_SPARK; break;
                case JUGGLE_BALL: speed = SPEED_BALL;  break;
                default:          speed = SPEED_PROJECTILE; break;
            }
            if (p.hitting) {
                p.hitTimer += dt;
                if (p.hitTimer >= HIT_DURATION) p.alive = false;   // animation finished: remove it
                continue;                                           // a hitting shot doesn't move or hit again
            }
            if(p.fromPlayer)
                p.x = p.x +speed * dt;
            else
                p.x = p.x -speed * dt;

            if(p.x<LANE_LEFT || p.x>SCREEN_W){
                p.alive=false;
                continue;
            }
            if (p.fromPlayer) {
                for (Enemy& e : enemies) {
                    if (!e.alive || e.lane != p.lane) continue;               // only living ones in this lane
                    if (std::abs(e.x - p.x) <= HIT_RANGE) {                   // touching
                        e.health -= DMG_PLAYER;
                        p.hitting = true;                                     // start the hit animation
                        p.hitTimer = 0.0f;
                        break;                                                // one shot hits one target
                    }
                }
            }
            else {
                for (Player& pl : players) {
                    if (!pl.alive || pl.lane != p.lane) continue;
                    if (std::abs(pl.x - p.x) > HIT_RANGE) continue;           // not touching yet

                    // Dodging: the shot passes this guard
                    if (p.height == HIGH && pl.state == DUCK) continue;       // spark flies over a ducking guard
                    if (p.height == LOW  && pl.state == JUMP) continue;       // ball rolls under a jumping guard

                    pl.health -= DMG_ENEMY;
                    p.hitting = true;
                    p.hitTimer = 0.0f;
                    break;
                }
            }
        }
    }
}
