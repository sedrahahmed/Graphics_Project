#include "Shots.h"
#include "Game.h"
#include <cmath>

static const float HIT_RANGE = 35; //how close a shot must be to a character to count as touching

void updateShots(float dt) {
    for(Projectile& p:shots){
        if(p.alive){
            float speed;
             if (p.hitting) {
                p.hitTimer += dt;
                if (p.hitTimer >= HIT_DURATION) p.alive = false;//if hit time finished remove it
                continue;
            }
            switch (p.type) {//pick the speed acording to the type
                case LAMA_DART: speed = SPEED_DART;  break;
                case MAGICIAN_HIT: speed = SPEED_SPARK; break;
                case JUGGLE_BALL: speed = SPEED_BALL;  break;
                default:          speed = SPEED_PROJECTILE; break; 
            }
           
            if(p.fromPlayer)
                p.x = p.x +speed * dt; //players shots go right
            else
                p.x = p.x -speed * dt;//enemys shots go left

            if(p.x<LANE_LEFT || p.x>SCREEN_W){ //if the shot leaves the lane area remove it
                p.alive=false;
                continue;
            }
            if (p.fromPlayer) {
                for (Enemy& e : enemies) {
                    if (!e.alive || e.lane != p.lane) continue;//only living ones in this lane
                    if (std::abs(e.x - p.x) <= HIT_RANGE) { //touching
                        e.health -= DMG_PLAYER;
                        p.hitting = true;//start the hit animation
                        p.hitTimer = 0.0f;
                        break; //one shot hits one enemy
                    }
                }
            }
            else {
                for (Player& pl : players) {
                    if (!pl.alive || pl.lane != p.lane) continue;
                    if (std::abs(pl.x - p.x) > HIT_RANGE) continue; //not touching

                  
                    if (p.height == HIGH && pl.state == DUCK) continue;//flies over a ducking guard
                    if (p.height == LOW  && pl.state == JUMP) continue;//rolls under a jumping guard

                    pl.health -= DMG_ENEMY;
                    p.hitting = true;
                    p.hitTimer = 0.0f;
                    break;
                }
            }
        }
    }
}
