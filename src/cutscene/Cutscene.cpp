
#include "Cutscene.h"
#include "Draw.h"
#include "Colors.h"
#include "Text.h"
#include "Sound.h"
#include "Config.h"
#include "CharacterDraw.h"
#include "Background.h"
#include "Environment.h"
#include <cmath>

static float cutsceneTime = 0.0f;
static float previousCutsceneTime = 0.0f;   // for "play sound once at moment T" checks
static unsigned int ownerTexture = 0, shadowArmTexture = 0, ticketTexture = 0,
                    posterTexture = 0, wheelTexture = 0, visitorTexture = 0, cassetteTexture = 0;

//timeline
static const float SCENE1_START = 0.0f;
static const float SCENE2_START = 3.0f;   
static const float SCENE3_START = 7.0f;    
static const float SCENE4_START = 8.5f;
static const float SCENE5_START = 11.0f;
static const float SCENE6_END   = 20.0f;

// scene 1 hand-shake choreography
static const float ARMS_MEET_T = 2.0f;       // arms touch -> red flash + ticket
static const float ARMS_MEET_X   = 640.0f;  // meeting point, pixels from left



// helpers

static float laneFloorY(int lane) {  //y value of the bottom of the lane for a given lane index
    return LANE_BOTTOM + LANE_HEIGHT * lane + LANE_HEIGHT / 2.0f;
}

// 0 -> 1 as cutsceneTime goes from t0 to t1 (0 before t0, 1 after t1).
// Used with lerp to move things: x = lerp(fromX, toX, progressBetween(t0, t1))
static float progressBetween(float t0, float t1) {
    if (t1 <= t0) return cutsceneTime >= t1 ? 1.0f : 0.0f;
    float p = (cutsceneTime - t0) / (t1 - t0);
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    return p;
}

// the point p of the way from 'from' to 'to' (p = 0 -> from, p = 1 -> to)
static float lerp(float from, float to, float p) { return from + (to - from) * p; }

static void drawFullscreenRect(float r, float g, float b, float a) {
    drawRect(SCREEN_W / 2.0f, SCREEN_H / 2.0f, (float)SCREEN_W, (float)SCREEN_H, 0, glm::vec4(r, g, b, a));
}

// the night look: after the background, a full-screen dark purple rectangle (alpha 0.55)
static void drawNightOverlay() {
    drawFullscreenRect(0.10f, 0.03f, 0.18f, 0.55f);
}

//local = time since the start of the scene & sceneLength = total length of the scene
static void drawSceneFades(float local, float sceneLength) {  
    float fadeIn  = 1.0f - local / 0.30f;                      //how dark based on how far into the scene we are
    float fadeOut = (local - (sceneLength - 0.30f)) / 0.30f;
    float a;
    if(fadeIn > fadeOut)                                    //use the larger of the two to fade in and out as the alpha value
        a= fadeIn;
    else
        a= fadeOut;    
    if (a > 1.0f) a = 1.0f;
    if (a > 0.001f) drawFullscreenRect(0, 0, 0, a);
}

// one centerline subtitle: white text with a black shadow so it reads anywhere
static void subtitle(const std::string& s, float y, float scale) {
    glm::vec4 shadow(0, 0, 0, 0.85f);
    float x = SCREEN_W / 2.0f - textWidth(s, scale) / 2.0f;
    drawText(s, x + 2, y - 2, scale, shadow);
    drawText(s, x, y, scale, WHITE);
}
static void subtitleOneLine(const std::string& s)  { subtitle(s, 46.0f, 0.55f); }
static void subtitleTwoLines(const std::string& a, const std::string& b) {
    subtitle(a, 78.0f, 0.52f);
    subtitle(b, 42.0f, 0.52f);
}


static void drawTicket(float x, float y, float t) {
    float r = 20.0f + 6.0f * sinf(t * 6.0f);                        // pulsing glow
    drawCircle(x, y, r, glm::vec4(0.98f, 0.80f, 0.20f, 0.30f));
    drawSprite(ticketTexture, 0, 1, x, y, 48.0f, 36.0f, 0, glm::vec4(1.0f), false);
}

static float handshakeY() { return 280.0f; }   // owner arm height above the ground

static void drawStringLights(bool flickerOn) {
    if (flickerOn && (int)(cutsceneTime * 10) % 3 == 0) return;
    glm::vec4 wire(0.05f, 0.03f, 0.06f, 1.0f);
    glm::vec4 bulb(1.0f, 0.85f, 0.50f, 0.55f);
    float x0 = 40.0f, x1 = 1240.0f, y = 655.0f;
    drawLine(x0, y, x1, y - 5.0f, 2.0f, wire);
    for (int i = 0; i < 10; i++) {
        float x = x0 + i * (x1 - x0) / 9.0f;
        float hang = 10.0f + 14.0f * (0.5f + 0.5f * sinf(i * 1.7f)); // uneven sag
        drawLine(x, y, x, y - hang, 2.0f, wire);
        drawCircle(x, y - hang - 6.0f, 7.0f, bulb);
    }
}

// cassette recorder: dark grey rectangle + two reels; the PLAY triangle lights up
static void drawCassette(float x, float y, bool playing) {
    drawSprite(cassetteTexture, playing ? 1 : 0, 2, x, y, 40.0f, 26.0f, 0, glm::vec4(1.0f), false);
}

// pairs of small red circles, 8 px apart, at the cursed performers' face height
static void drawRedEyes(float x, float faceY, float alpha) {
    glm::vec4 eye = CURSE_RED; eye.a = alpha;
    drawCircle(x - 4.0f, faceY, 3.5f, eye);
    drawCircle(x + 4.0f, faceY, 3.5f, eye);
}



static void drawStageBase(float t, bool withBooth, bool lights, bool lightsFlicker) {
    drawBackground(t);
    drawNightOverlay();
    drawEnvironment();
    // if (withBooth) {
    //     if (boothTexture == 0) boothTexture = loadTexture(ASSET_DIR "background/Ticket_booth.png");
    //     drawSprite(boothTexture, 0, 1, 105.0f, 183.5f, 210.0f, 315.0f, 0.0f, glm::vec4(1.0f), false);

    //     // the Golden Ticket in the picture glows
    //     drawCircle(108.0f, 128.0f, 26.0f + 6.0f * sinf(t * 6.0f), glm::vec4(0.98f, 0.80f, 0.20f, 0.25f));
    // }
    if (lights) drawStringLights(lightsFlicker);
}


// performers stand in place (frame 0 of their standing strip): ATTACK + stateTime 0
static void drawPerformer(EnemyType type, float x, int lane, float t, float healthFrac) {
    drawEnemy(x, laneFloorY(lane), type, t, ATTACK, 0.0f, healthFrac);
}



static void drawScene1(float t) {
    float local = t - SCENE1_START;
    drawStageBase(t, false, true, false);

    // owner stands at x = 584 and reaches right shadow arm grows in from the right edge
    float armP = progressBetween(SCENE1_START, ARMS_MEET_T);
    int ownerFrame = (int)(armP * 3.99f);                       
    drawSprite(ownerTexture, ownerFrame, 4, 584.0f, 286.0f, 180.0f, 240.0f, 0, glm::vec4(1.0f), false);

    int armFrame = (int)(armP * 5.99f);                          
    drawSprite(shadowArmTexture, armFrame, 6, 967.0f, 280.0f, 665.0f, 133.0f, 0, glm::vec4(1.0f), false);
    // from the handshake: red flash (alpha 0.6 -> 0) and the ticket glows in his hand
    if (t >= ARMS_MEET_T) {
        float flash = 0.6f * (1.0f - progressBetween(ARMS_MEET_T, ARMS_MEET_T + 0.3f));
        drawFullscreenRect(1.0f, 0.0f, 0.0f, flash);
    }
    if (t >= ARMS_MEET_T - 0.2f) drawTicket(ARMS_MEET_X, handshakeY(), t);

    drawSceneFades(local, SCENE2_START - SCENE1_START);
    
    subtitleTwoLines("Desperate to save his dying carnival, the owner made a deal...",
                     "...and sealed it with a Golden Ticket.");
}



static void drawScene2(float t) {
    float local = t - SCENE2_START;
    drawStageBase(t, false, true, true);       // the lights flicker only in this scene

    // Clown, Jester and Magician side by side (standing strips, normal colors)
// from 4.0s they tint blood-red (damageTint via healthFrac 1 -> 0 until 5.0s)   
     float frac = 1.0f - progressBetween(4.0f, 5.0f);   
    float eyeA = progressBetween(4.0f, 4.3f);          
    struct { EnemyType type; float x; } trio[3] = { { CLOWN, 520.0f }, { JESTER, 640.0f }, { MAGICIAN, 760.0f } };
    int lane = 2;
    for (int i = 0; i < 3; i++) {
        drawPerformer(trio[i].type, trio[i].x, lane, t, frac);
        if (eyeA > 0.01f) drawRedEyes(trio[i].x, laneFloorY(lane) + 85.0f, eyeA);
    }


    drawSceneFades(local, SCENE3_START - SCENE2_START);
    subtitleTwoLines("The deal brought the crowds back.",
                    "But at night, its performers became something else.");
}


static void drawScene3(float t) {
    drawBackground(t);
    drawEnvironment();
    drawFullscreenRect(1.0f, 0.93f, 0.55f, 0.30f);          // daytime wash

    // the poster grows slightly (1.0 -> 1.08) so the shot isn't frozen
    float zoom = 1.0f + 0.08f * progressBetween(SCENE3_START, SCENE4_START);

    // the poster in front of everything
    drawSprite(posterTexture, 0, 1, 640.0f, 410.0f, 368.0f * zoom, 440.0f * zoom, 0, glm::vec4(1.0f), false);

    // a dark banner under it for the details
    drawRect(640.0f, 130.0f, 520.0f, 110.0f, 0, glm::vec4(0.05f, 0.02f, 0.08f, 0.8f));
    float s = 0.6f;
    drawText("GENEROUS PAY!", 640.0f - textWidth("GENEROUS PAY!", s) / 2.0f, 140.0f, s, TICKET_GOLD);
    drawText("12:00 AM - 6:00 AM", 640.0f - textWidth("12:00 AM - 6:00 AM", s) / 2.0f, 95.0f, s, WHITE);
}


//guard information for the cutscene: they walk left at a steady speed, then stop at their posts at STOP_TIME
static const float CHASE_SPEED     = 130.0f;   
static const float PERFORMER_SPEED = 130.0f;   
static const float STOP_TIME       = 16.0f;    
static const float SEIF_POST_X = 320.0f, JANA_POST_X = 460.0f, LAMA_POST_X = 400.0f;
static const float JESTER_STOP_X = 700.0f, CLOWN_STOP_X = 680.0f, MAGICIAN_STOP_X = 820.0f;

// x of someone who walks left at a steady speed and stops at stopX at STOP_TIME
static float chaseX(float stopX, float t, float speed = CHASE_SPEED) {
    float timeLeft = STOP_TIME - t;
    if (timeLeft < 0.0f) timeLeft = 0.0f;
    return stopX + speed * timeLeft;
}


static void drawScene4(float t) {
    float local = t - SCENE4_START;
    drawStageBase(t, true, true, false);

    // guards walk left, facing left; they keep going at the same speed into scene 5
    float seifX = chaseX(SEIF_POST_X, t);
    float janaX = chaseX(JANA_POST_X, t);
    float lamaX = chaseX(LAMA_POST_X, t);
    bool walking = true;
    AnimState st = WALK;

    // back lane first so the front lane is drawn over it
    drawGuard(lamaX, laneFloorY(3), RANGED_LAMA, t, st, 0.0f, 1.0f, walking);
    drawGuard(seifX, laneFloorY(1), MELEE_SEIF,  t, st, 0.0f, 1.0f, walking);   // face left while walking
    drawGuard(janaX, laneFloorY(1), DEFENSE_JANA, t, st, 0.0f, 1.0f, walking);

    drawCassette(150.0f, 157.0f, t >= 10.6f);  // on the booth counter; PLAY pressed at 10.6s


    drawSceneFades(local, 100.0f);   // fade in only: scene 5 continues the same shot, no black dip
    subtitleTwoLines("Three new guards took the night shift,",
                    "unaware of what happened to the ones before.");
}




// Draws everything of scene 5 as a pure function of t. Scene 6 reuses it as the
// game-stage the Night One card fades away from. Subtitles drawn separately.
static void drawScene5Actors(float t) {
    float laneY1 = laneFloorY(1);
    float laneY2 = laneFloorY(2);
    float laneY3 = laneFloorY(3);
    // lanes: 1 = Seif, Jana, Jester | 2 = Magician | 3 = Lama, Clown (ball + jump)

    // 11-16 (while the owner talks): the guards run back toward the booth facing left,
    // with the performers chasing. At 16 they reach their posts and turn to face them.
    bool running = t < STOP_TIME;
    AnimState guardSt = running ? WALK : ATTACK;
    float seifX = chaseX(SEIF_POST_X, t);   // same formula as scene 4, so no jump between scenes
    float janaX = chaseX(JANA_POST_X, t);
    float lamaX = chaseX(LAMA_POST_X, t);

    drawGuard(seifX, laneY1, MELEE_SEIF, t, guardSt, 0.0f, 1.0f, running);
    if (t < 16.3f) drawGuard(janaX, laneY1, DEFENSE_JANA, t, guardSt, 0.0f, 1.0f, running);
    else           drawGuard(janaX, laneY1, DEFENSE_JANA, t, ATTACK, t - 16.3f, 1.0f);   // Jana braces


    // the performers chase in from off-screen right (at 11 s they're at x 1330+),
    // gaining on the guards, and stop close behind them at STOP_TIME
    bool chasing = t < STOP_TIME;
    float clownX  = chaseX(CLOWN_STOP_X,    t, PERFORMER_SPEED);    // lane 3
    float jesterX = chaseX(JESTER_STOP_X,   t, PERFORMER_SPEED);    // lane 1
    float magX    = chaseX(MAGICIAN_STOP_X, t, PERFORMER_SPEED);    // lane 2

    // the Jester lunges toward Jana from 16.3 (lands just in front of her by 16.8)
    float lungeP = 0.0f;
    if (t >= 16.0f) {
        lungeP = progressBetween(16.3f, 16.8f);
        jesterX = lerp(jesterX, JANA_POST_X + 80.0f, lungeP);
    }

    // clown throws the ball at Lama at 16.2 (250 px/s like in gameplay, leftwards).
    if (t >= 16.2f) {
        float ballX = (CLOWN_STOP_X - 20.0f) - 250.0f * (t - 16.2f);
        if (ballX > LANE_LEFT) drawJuggleBall(ballX, laneY3, t, 0.0f);
    }

    if (t >= 17.0f) {
        float jp = (t - 17.0f) / 0.6f;
        if (jp > 1.0f) jp = 1.0f;
        float bob = sinf(jp * 3.1415926f) * 42.0f;                  // small hop arc
        drawGuard(LAMA_POST_X, laneY3 + bob, RANGED_LAMA, t, JUMP, t - 17.0f, 1.0f);
    } else {
        drawGuard(lamaX, laneY3, RANGED_LAMA, t, guardSt, 0.0f, 1.0f, running);
    }

    //eniemies
    AnimState clownSt = WALK;
    float clownStateTime = 0.0f;
    if (t >= 16.0f) { clownSt = ATTACK; clownStateTime = 0.0f; }
    if (t >= 16.2f) { clownStateTime = t - 16.2f; }                 // throw animation plays
    drawEnemy(clownX, laneY3, CLOWN, t, clownSt, clownStateTime, 1.0f);
    

    AnimState jesterSt = (chasing || (t >= 16.3f && lungeP < 1.0f)) ? WALK : ATTACK;
    float jesterStateTime = (jesterSt == ATTACK) ? t - 16.8f : 0.0f;
    drawEnemy(jesterX, laneY1, JESTER, t, jesterSt, jesterStateTime, 1.0f);


    AnimState magSt = chasing ? WALK : ATTACK;
    float magStateTime = (magSt == ATTACK) ? t - 16.0f : 0.0f;
    drawEnemy(magX, laneY3, MAGICIAN, t, magSt, magStateTime, 1.0f);

}

static void drawScene5(float t) {
    float local = t - SCENE5_START;
    drawStageBase(t, true, true, false);
    drawScene5Actors(t);
    // (no tape static over the stage: the chase is shown clean while the owner talks)
    // no fade here: it continues straight on from scene 4

    if (t < 12.6f)       subtitleOneLine("Owner: Hello, hello! Welcome to the carnival!");
    else if (t < 14.2f)  subtitleOneLine("Owner: Our performers... wander after closing.");
    else if (t < 16.0f)  subtitleOneLine("Owner: Don't let them reach the Golden Ticket.");
    else if (t >= 17.2f) subtitleOneLine("Jana: Stay behind me!");
    else if (t >= 16.0f) subtitleOneLine("Jester: The Golden Ticket... our way out!");
}

static void drawScene6(float t) {
    // the stage stays until the card is gone: scene 5's actors frozen behind
    drawStageBase(t, true, true, false);
    drawScene5Actors(t);

    // fade to black 18.5-19.0, card 19.0-19.7, everything fades back to the stage 19.7-20.0
    float blackA = progressBetween(18.5f, 19.0f);
    if (t >= 19.7f) blackA = 1.0f - progressBetween(19.7f, 20.0f);
    float cardA = 0.0f;
    if (t >= 19.0f) cardA = progressBetween(19.0f, 19.35f) * (t >= 19.7f ? 1.0f - progressBetween(19.7f, 20.0f) : 1.0f);

    drawFullscreenRect(0, 0, 0, blackA);

    if (cardA > 0.001f) {
        glm::vec4 gold = TICKET_GOLD; gold.a = cardA;
        glm::vec4 white = WHITE;      white.a = cardA;
        glm::vec4 dim(0.9f, 0.9f, 0.9f, cardA);
        glm::vec4 shadow(0, 0, 0, 0.9f * cardA);
        // title card centered with textWidth
        float x1 = SCREEN_W / 2.0f - textWidth("12:00 AM", 1.5f) / 2.0f;
        float x2 = SCREEN_W / 2.0f - textWidth("NIGHT ONE", 1.0f) / 2.0f;
        float x3 = SCREEN_W / 2.0f - textWidth("SURVIVE UNTIL 6:00 AM", 0.7f) / 2.0f;
        drawText("12:00 AM", x1 + 2, 428.0f, 1.5f, shadow);
        drawText("12:00 AM", x1, 430.0f, 1.5f, gold);
        drawText("NIGHT ONE", x2 + 2, 348.0f, 1.0f, shadow);
        drawText("NIGHT ONE", x2, 350.0f, 1.0f, white);
        drawText("SURVIVE UNTIL 6:00 AM", x3, 295.0f, 0.7f, dim);
    }
}


void startCutscene() {
    cutsceneTime = 0.0f;
    previousCutsceneTime = 0.0f;
    if (ownerTexture == 0) {   // only the first time the cutscene starts
        ownerTexture     = loadTexture(ASSET_DIR "background/Owner.png");
        shadowArmTexture = loadTexture(ASSET_DIR "background/shadow_arm_strip.png");
        ticketTexture    = loadTexture(ASSET_DIR "background/golden_ticket.png");
        posterTexture    = loadTexture(ASSET_DIR "background/job_poster.png");
        cassetteTexture  = loadTexture(ASSET_DIR "background/cassette.png");
    }
}

void updateCutscene(float dt) {
    if (dt > 0.1f) dt = 0.1f;                  // tab-out gaps shouldn't skip scenes
    previousCutsceneTime = cutsceneTime;
    cutsceneTime += dt;

    // Sounds: fired once, the moment their cue time is crossed.
    // (rumble / thud / deep hit / wind / footsteps / static / final note are
    //  TODO when the team drops the extra files in assets/sounds.)
    if (previousCutsceneTime < 10.6f && cutsceneTime >= 10.6f) playSoundEffect(SOUND_CASSETTE);      // PLAY pressed
    if (previousCutsceneTime < 11.0f && cutsceneTime >= 11.0f) {                                     // tape starts
        stopMusic();
        playSoundEffect(SOUND_OWNER_VOICE);
    }
    if (previousCutsceneTime < 16.0f && cutsceneTime >= 16.0f) {                                     // midnight: guards turn, fight starts
        playMusic();                               // carnival music back on (creepier feel: TODO)
        playSoundEffect(SOUND_CLOCK);
    }
    if (previousCutsceneTime < 16.2f && cutsceneTime >= 16.2f) playSoundEffect(SOUND_ROLLING_BALL);  // ball thrown
    if (previousCutsceneTime < 16.3f && cutsceneTime >= 16.3f) playSoundEffect(SOUND_MELEE_ATTACK);  // Jester lunges
}

void drawCutscene() {
    if      (cutsceneTime < SCENE2_START) drawScene1(cutsceneTime);
    else if (cutsceneTime < SCENE3_START) drawScene2(cutsceneTime);
    else if (cutsceneTime < SCENE4_START) drawScene3(cutsceneTime);
    else if (cutsceneTime < SCENE5_START) drawScene4(cutsceneTime);
    else if (cutsceneTime < 18.5f)       drawScene5(cutsceneTime);
    else                                 drawScene6(cutsceneTime);
}

void skipCutscene() {
    cutsceneTime = SCENE6_END;                  // jumping straight to the game
    previousCutsceneTime = SCENE6_END;          // swallow any pending sound cues
}

bool isCutsceneFinished() { return cutsceneTime >= SCENE6_END; }
