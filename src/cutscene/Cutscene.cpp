// Cutscene.cpp. Owner: Person D
// The 20-second intro (script v2: "Golden Ticket"). Seven beats on ONE clock:
//
//   0.0- 3.0  The deal          owner + shadow handshake, red flash, gold ticket
//   3.0- 7.0  The curse         performers turn red, lights flicker
//   7.0- 8.5  The job           daytime, poster
//   8.5-11.0  The first night   booth, cassette, guards walk in and stop, PLAY pressed
//  11.0-16.0  The chase         owner's three lines play while the guards run left
//                               to their posts with the performers chasing them
//  16.0-18.5  The curse awakens guards turn around: ball vs Lama, Jester vs Jana
//  18.5-20.0  Night One         fade to black, title card, fades into the game
//
// Every position / tint / subtitle is computed from cutsceneTime each frame,
// so nothing per-character is stored. See "Carnival: Cutscene Script v2".

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
static unsigned int boothTexture = 0;   // Ticket_booth.png, loaded the first time it's drawn
static unsigned int ownerTexture = 0, shadowArmTexture = 0, ticketTexture = 0,
                    posterTexture = 0, wheelTexture = 0, visitorTexture = 0, cassetteTexture = 0;

// ---------------- timeline (seconds) ----------------
static const float BEAT1_START = 0.0f;
static const float BEAT2_START = 3.0f;   
static const float BEAT3_START = 7.0f;    // beat 2 got 1 extra second
static const float BEAT4_START = 8.5f;
static const float BEAT5_START = 11.0f;
static const float BEAT6_START = 11.0f;
static const float BEAT7_END   = 20.0f;

// beat 1 hand-shake choreography
static const float ARMS_MEET_T = 2.0f;       // arms touch -> red flash + ticket
static const float ARMS_MEET_X   = 640.0f;  // meeting point, pixels from left

// ---------------- tiny helpers ----------------

// y of the floor of a lane (lane 0 = bottom). The script's "laneFloorY".
static float laneFloorY(int lane) {
    return LANE_BOTTOM + LANE_HEIGHT * lane + LANE_HEIGHT / 2.0f;
}

// 0..1 progress of cutsceneTime between t0 and t1 (clamped), for the classic
//   x = fromX + (toX - fromX) * progress
// move used by the arms, walkers, the Jester's lunge and the ball.
static float progressBetween(float t0, float t1) {
    if (t1 <= t0) return cutsceneTime >= t1 ? 1.0f : 0.0f;
    float p = (cutsceneTime - t0) / (t1 - t0);
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    return p;
}

static float lerp(float from, float to, float p) { return from + (to - from) * p; }

static void drawFullscreenRect(float r, float g, float b, float a) {
    drawRect(SCREEN_W / 2.0f, SCREEN_H / 2.0f, (float)SCREEN_W, (float)SCREEN_H, 0, glm::vec4(r, g, b, a));
}

// the night look: after the background, a full-screen dark purple rectangle (alpha 0.55)
static void drawNightOverlay() {
    drawFullscreenRect(0.10f, 0.03f, 0.18f, 0.55f);
}

// a scene starts fading from black (1 -> 0) and ends fading to black (0 -> 1)
// a scene starts fading from black (1 -> 0) and ends fading to black (0 -> 1)
static void drawBeatFades(float local, float beatLength) {
    float fadeIn  = 1.0f - local / 0.30f;                      // 1 -> 0 over the first 0.3 s
    float fadeOut = (local - (beatLength - 0.30f)) / 0.30f;    // 0 -> 1 over the last 0.3 s
    float a = fadeIn > fadeOut ? fadeIn : fadeOut;             // whichever is darker
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

// the Golden Ticket: small gold rectangle + pulsing transparent gold circle behind it
static void drawTicket(float x, float y, float t) {
    float r = 20.0f + 6.0f * sinf(t * 6.0f);                        // pulsing glow
    drawCircle(x, y, r, glm::vec4(0.98f, 0.80f, 0.20f, 0.30f));
    drawSprite(ticketTexture, 0, 1, x, y, 48.0f, 36.0f, 0, glm::vec4(1.0f), false);
}

// ================= props built from shapes =================

// height of the handshake (where the owner's arm and the shadow arm meet)
static float handshakeY() { return 280.0f; }   // owner arm height above the ground

// The owner: black silhouette (rect body, circle head, two rects top hat, one rect arm).
// armP: 0..1 how far his arm has reached toward the meeting point.
static void drawOwner(float x, float feetY, float armP) {
    glm::vec4 ink(0.03f, 0.02f, 0.05f, 1.0f);
    drawRect(x, feetY + 75.0f, 66.0f, 150.0f, 0, ink);              // body
    drawCircle(x, feetY + 172.0f, 22.0f, ink);                      // head
    drawRect(x, feetY + 190.0f, 52.0f,  6.0f, 0, ink);              // top-hat brim
    drawRect(x, feetY + 202.0f, 32.0f, 20.0f, 0, ink);              // top-hat crown
    // his right arm reaching toward the handshake point:
    float shoulderY = feetY + 120.0f;
    float shoulderX = x + 22.0f;
    float handX = lerp(shoulderX + 6.0f, ARMS_MEET_X, armP);
    drawRect((shoulderX + handX) / 2.0f, shoulderY, handX - shoulderX, 11.0f, 0, ink);
    drawCircle(handX, shoulderY, 6.0f, ink);                        // hand
}

// The shadow's arm: long black rectangle from the right edge, triangle claws at the
// tip. It slides in at the same height as the owner's arm until they touch.
static void drawShadowArm(float armP) {
    glm::vec4 ink(0.02f, 0.01f, 0.03f, 1.0f);
    float tipX = lerp(SCREEN_W, ARMS_MEET_X, armP);
    float armY = handshakeY();
    drawRect((tipX + SCREEN_W) / 2.0f, armY, SCREEN_W - tipX, 13.0f, 0, ink);
    drawTriangle(tipX - 9.0f, armY, 18.0f, 10.0f, -90.0f, ink);     // claws point left
}

static void drawStringLights(bool flickerOn) {
    // flicker: skip the light drawing when (int)(cutsceneTime * 10) % 3 == 0 (beat 2)
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

// Ferris wheel: circle outline + 8 spokes (drawLine), rotating with time.
static void drawFerrisWheel(float cx, float cy, float t) {
    glm::vec4 ink(0.10f, 0.05f, 0.12f, 1.0f);
    glm::vec4 cabins[3] = { CURSE_RED, CURSE_PURPLE, CURSE_GREEN };
    float r = 105.0f;
    const float PI2 = 6.2831853f;
    // ring (outline made of short segments)
    for (int i = 0; i < 28; i++) {
        float a1 = i * PI2 / 28.0f, a2 = (i + 1) * PI2 / 28.0f;
        drawLine(cx + cosf(a1) * r, cy + sinf(a1) * r, cx + cosf(a2) * r, cy + sinf(a2) * r, 3.0f, ink);
    }
    // spokes turn slowly with time; each spoke end carries a cabin
    for (int i = 0; i < 8; i++) {
        float a = t * 0.6f + i * PI2 / 8.0f;
        float px = cx + cosf(a) * r, py = cy + sinf(a) * r;
        drawLine(cx, cy, px, py, 3.0f, ink);
        drawRect(px, py - 11.0f, 14.0f, 18.0f, 0, cabins[i % 3]);   // cabin hangs under the tip
    }
    // legs down to the boardwalk
    drawLine(cx - 78.0f, 405.0f, cx, cy,  4.0f, ink);
    drawLine(cx + 78.0f, 405.0f, cx, cy,  4.0f, ink);
}

// crowd silhouettes: dark rounded rectangles of different heights, sliding left -> right
static void drawCrowd(float t) {
    glm::vec4 sil(0.05f, 0.03f, 0.07f, 0.92f);
    for (int i = 0; i < 10; i++) {
        float h = 55.0f + 50.0f * (0.5f + 0.5f * sinf(i * 9.7f));    // heights 55-105
        float x = fmodf(40.0f + i * 137.0f + t * 40.0f, 1360.0f) - 40.0f;
        drawRect(x, 405.0f + h / 2.0f, 34.0f, h, 0, sil);
        drawCircle(x, 405.0f + h + 8.0f, 12.0f, sil);               // head
    }
}

// the job poster: cream rectangle, red border, three lines of text (the poster IS the text)
static void drawJobPoster(float cx, float cy) {
    drawRect(cx, cy, 208.0f, 96.0f, 0, CURSE_RED);                  // border
    drawRect(cx, cy, 198.0f, 86.0f, 0, glm::vec4(0.97f, 0.94f, 0.85f, 1.0f));
    glm::vec4 ink(0.25f, 0.08f, 0.10f, 1.0f);
    float s = 0.28f;
    drawText("NIGHT SECURITY", cx - textWidth("NIGHT SECURITY", s) / 2.0f, cy + 20.0f, s, ink);
    drawText("WANTED",           cx - textWidth("WANTED", s) / 2.0f,           cy -  2.0f, s, ink);
    drawText("12:00 AM - 6:00 AM", cx - textWidth("12:00 AM - 6:00 AM", s) / 2.0f, cy - 28.0f, s, ink);
    drawText("GENEROUS PAY!", cx - textWidth("GENEROUS PAY!", s) / 2.0f, cy - 62.0f, s * 0.9f, ink);
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

// static: faint horizontal lines flickering over the black recording screen
static void drawTapeStatic(float t) {
    for (int i = 0; i < 26; i++) {
        float y = 18.0f + i * 26.5f;
        bool bright = sinf(i * 12.9898f + t * 7.3f) > 0.7f;
        float a = bright ? 0.10f : 0.05f;
        drawRect(SCREEN_W / 2.0f, y, (float)SCREEN_W, 1.2f, 0, glm::vec4(1, 1, 1, a));
    }
    int frame = (int)(t * 14.0f);
    if (frame % 9 == 0) {                                           // occasional noise band
        float y = 40.0f + (float)((frame * 37) % 640);
        drawRect(SCREEN_W / 2.0f, y, (float)SCREEN_W, 8.0f, 0, glm::vec4(1, 1, 1, 0.08f));
    }
}

// ================= staged pieces shared by beats =================

// game stage pieces: drawn in this order in the game too (bg first, overlay, then props)
static void drawStageBase(float t, bool withBooth, bool lights, bool lightsFlicker) {
    drawBackground(t);
    drawNightOverlay();
    drawEnvironment(t);
    // if (withBooth) {
    //     if (boothTexture == 0) boothTexture = loadTexture(ASSET_DIR "background/Ticket_booth.png");
    //     drawSprite(boothTexture, 0, 1, 105.0f, 183.5f, 210.0f, 315.0f, 0.0f, glm::vec4(1.0f), false);

    //     // the Golden Ticket in the picture glows
    //     drawCircle(108.0f, 128.0f, 26.0f + 6.0f * sinf(t * 6.0f), glm::vec4(0.98f, 0.80f, 0.20f, 0.25f));
    // }
    if (lights) drawStringLights(lightsFlicker);
}

// the moon "rises": the environment already has Moon.png top-right; a glow behind it
// brightens like a rise (beats 2 / 4 / 6)
static void drawMoonRise(float t) {
    //float glow = 0.35f * progressBetween(3.2f, 4.5f);       // beat 2 only
    //drawCircle(1162.0f, 598.0f, 60.0f, glm::vec4(0.98f, 0.94f, 0.75f, glow));
}

// performers stand in place (frame 0 of their standing strip): ATTACK + stateTime 0
static void drawPerformer(EnemyType type, float x, int lane, float t, float healthFrac) {
    drawEnemy(x, laneFloorY(lane), type, t, ATTACK, 0.0f, healthFrac);
}

// guards stand at post: their attack strip frame 0, full health
static void drawGuardAtPost(PlayerType type, float x, int lane, float t, float stateTime) {
    drawGuard(x, laneFloorY(lane), type, t, ATTACK, stateTime, 1.0f);
}

// ================= BEAT 1: the deal (0.0 - 2.5) =================

static void drawBeat1(float t) {
    float local = t - BEAT1_START;
    drawStageBase(t, false, true, false);

    // owner stands at x = 500; shadow arm reaches in from the right edge
    float armP = progressBetween(BEAT1_START, ARMS_MEET_T);
    int ownerFrame = (int)(armP * 3.99f);                       // 0,1,2,3: frame 3 = hand out
    drawSprite(ownerTexture, ownerFrame, 4, 584.0f, 286.0f, 180.0f, 240.0f, 0, glm::vec4(1.0f), false);

    int armFrame = (int)(armP * 5.99f);                          // 0..5: the arm grows longer
    drawSprite(shadowArmTexture, armFrame, 6, 967.0f, 280.0f, 665.0f, 133.0f, 0, glm::vec4(1.0f), false);
    // from the handshake: red flash (alpha 0.6 -> 0) and the ticket glows in his hand
    if (t >= ARMS_MEET_T) {
        float flash = 0.6f * (1.0f - progressBetween(ARMS_MEET_T, ARMS_MEET_T + 0.3f));
        drawFullscreenRect(1.0f, 0.0f, 0.0f, flash);
    }
    if (t >= ARMS_MEET_T - 0.2f) drawTicket(ARMS_MEET_X, handshakeY(), t);

    drawBeatFades(local, BEAT2_START - BEAT1_START);
    
    subtitleTwoLines("Desperate to save his dying carnival, the owner made a deal...",
                     "...and sealed it with a Golden Ticket.");
}

// ================= BEAT 2: the curse (2.5 - 5.0) =================

static void drawBeat2(float t) {
    float local = t - BEAT2_START;
    drawStageBase(t, false, true, true);       // the lights flicker only in this beat
    drawMoonRise(t);

    // Clown, Jester and Magician side by side (standing strips, normal colors)
    // from 3.5s they tint blood-red: damageTint via healthFrac 1 -> 0 into 4.5s
    float frac = 1.0f - progressBetween(4.0f, 5.0f);   // was 3.5f, 4.5f
    float eyeA = progressBetween(4.0f, 4.3f);          // was 3.5f, 3.8f
    struct { EnemyType type; float x; } trio[3] = { { CLOWN, 520.0f }, { JESTER, 640.0f }, { MAGICIAN, 760.0f } };
    int lane = 2;
    for (int i = 0; i < 3; i++) {
        drawPerformer(trio[i].type, trio[i].x, lane, t, frac);
        if (eyeA > 0.01f) drawRedEyes(trio[i].x, laneFloorY(lane) + 85.0f, eyeA);
    }


    drawBeatFades(local, BEAT3_START - BEAT2_START);
    subtitleTwoLines("The deal brought the crowds back.",
                    "But at night, its performers became something else.");
}

// ================= BEAT 3: the poster (6.0 - 7.5) =================

static void drawBeat3(float t) {
    drawBackground(t);
    drawEnvironment(t);
    drawFullscreenRect(1.0f, 0.93f, 0.55f, 0.30f);          // daytime wash

    // the poster grows slightly (1.0 -> 1.08) so the shot isn't frozen
    float zoom = 1.0f + 0.08f * progressBetween(BEAT3_START, BEAT4_START);
 

    // visitors walking left -> right, each with a different tint
    glm::vec4 tints[4] = { glm::vec4(1.0f), glm::vec4(0.8f, 0.9f, 1.0f, 1.0f),
                           glm::vec4(1.0f, 0.85f, 0.85f, 1.0f), glm::vec4(0.9f, 1.0f, 0.85f, 1.0f) };


    // the poster in front of everything
    drawSprite(posterTexture, 0, 1, 640.0f, 410.0f, 368.0f * zoom, 440.0f * zoom, 0, glm::vec4(1.0f), false);

    // a dark banner under it for the details
    drawRect(640.0f, 130.0f, 520.0f, 110.0f, 0, glm::vec4(0.05f, 0.02f, 0.08f, 0.8f));
    float s = 0.6f;
    drawText("GENEROUS PAY!", 640.0f - textWidth("GENEROUS PAY!", s) / 2.0f, 140.0f, s, TICKET_GOLD);
    drawText("12:00 AM - 6:00 AM", 640.0f - textWidth("12:00 AM - 6:00 AM", s) / 2.0f, 95.0f, s, WHITE);
}


// ================= guard formation (shared by beat 4 and beat 6) =================
// Everyone (guards AND performers) moves left at ONE steady speed and stops at
// STOP_TIME, when the owner finishes talking. Nobody speeds up, slows down or pauses.
// POST = where each guard stops. Lane 1: Jana in front, Seif behind her. Lane 2: Lama.
static const float CHASE_SPEED     = 130.0f;   // guards, pixels per second (start off-screen right in beat 4)
static const float PERFORMER_SPEED = 130.0f;   // performers: same speed, so the chase gap stays the same
static const float STOP_TIME       = 16.0f;    // owner's last line ends -> everyone stops, guards turn, fight starts
static const float SEIF_POST_X = 320.0f, JANA_POST_X = 460.0f, LAMA_POST_X = 400.0f;
static const float JESTER_STOP_X = 700.0f, CLOWN_STOP_X = 680.0f, MAGICIAN_STOP_X = 820.0f;

// x of someone who walks left at a steady speed and stops at stopX at STOP_TIME
static float chaseX(float stopX, float t, float speed = CHASE_SPEED) {
    float timeLeft = STOP_TIME - t;
    if (timeLeft < 0.0f) timeLeft = 0.0f;
    return stopX + speed * timeLeft;
}

// ================= BEAT 4: the first night (7.5 - 10.0) =================

static void drawBeat4(float t) {
    float local = t - BEAT4_START;
    drawStageBase(t, true, true, false);
    drawMoonRise(t);

    // guards walk left, facing left; they keep going at the same speed into beat 6
    float seifX = chaseX(SEIF_POST_X, t);
    float janaX = chaseX(JANA_POST_X, t);
    float lamaX = chaseX(LAMA_POST_X, t);
    bool walking = true;
    AnimState st = WALK;

    // back lane first so the front lane is drawn over it
    drawGuard(lamaX, laneFloorY(3), LANTERN_GUARD, t, st, 0.0f, 1.0f, walking);
    drawGuard(seifX, laneFloorY(1), MIRROR_GUARD,  t, st, 0.0f, 1.0f, walking);   // face left while walking
    drawGuard(janaX, laneFloorY(1), DEFENSE_GUARD, t, st, 0.0f, 1.0f, walking);

        drawCassette(150.0f, 157.0f, t >= 10.6f);  // on the booth counter; PLAY pressed at 10.6s


    drawBeatFades(local, 100.0f);   // fade in only: beat 6 continues the same shot, no black dip
    subtitleTwoLines("Three new guards took the night shift,",
                    "unaware of what happened to the ones before.");
}

// ================= BEAT 5: the recording (10.0 - 15.0) =================

static void drawBeat5(float t) {
    drawTapeStatic(t);                         // black screen, static flickering over it

    // the owner's three lines (owner_recording plays once at 10.0)
    if (t < 11.6f)      subtitleOneLine("Hello, hello! Welcome to the carnival!");
    else if (t < 13.2f) subtitleOneLine("Our performers... wander after closing.");
    else                subtitleOneLine("Don't let them reach the Golden Ticket.");
}

// ================= BEAT 6 + 7: stage, performers vs guards (15.0 - 20.0) =================

// Draws everything of beat 6 as a pure function of t. Beat 7 reuses it as the
// game-stage the Night One card fades away from. Subtitles drawn separately.
static void drawBeat6Actors(float t) {
    float laneY1 = laneFloorY(1);
    float laneY2 = laneFloorY(2);
    float laneY3 = laneFloorY(3);
    // lanes: 1 = Seif, Jana, Jester | 2 = Magician | 3 = Lama, Clown (ball + jump)

    // 11-16 (while the owner talks): the guards run back toward the booth facing left,
    // with the performers chasing. At 16 they reach their posts and turn to face them.
    bool running = t < STOP_TIME;
    AnimState guardSt = running ? WALK : ATTACK;
    float seifX = chaseX(SEIF_POST_X, t);   // same formula as beat 4, so no jump between beats
    float janaX = chaseX(JANA_POST_X, t);
    float lamaX = chaseX(LAMA_POST_X, t);

    drawGuard(seifX, laneY1, MIRROR_GUARD, t, guardSt, 0.0f, 1.0f, running);
    if (t < 16.3f) drawGuard(janaX, laneY1, DEFENSE_GUARD, t, guardSt, 0.0f, 1.0f, running);
    else           drawGuard(janaX, laneY1, DEFENSE_GUARD, t, ATTACK, t - 16.3f, 1.0f);   // Jana braces

    // three pairs of red eyes appear on the right at 11.0 (the performers, not in
    // yet). They fade out once the performers themselves come on screen.
    float peekA = progressBetween(11.0f, 11.2f) * (1.0f - progressBetween(11.4f, 11.7f));
    drawRedEyes(1290.0f, laneY1 + 85.0f, peekA);                    // Jester's pair
    drawRedEyes(1315.0f, laneY3 + 85.0f, peekA);                    // Clown's pair
    drawRedEyes(1335.0f, laneY2 + 85.0f, peekA);                    // Magician's pair

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
    // It reaches her at ~17.3, the middle of her jump (17.0 - 17.6), and dies at LANE_LEFT.
    if (t >= 16.2f) {
        float ballX = (CLOWN_STOP_X - 20.0f) - 250.0f * (t - 16.2f);
        if (ballX > LANE_LEFT) drawJuggleBall(ballX, laneY3, t, 0.0f);
    }

    // Lama jumps 17.0 - 17.6 (the exact control the player will press next).
    // Otherwise she just stands at her post.
    if (t >= 17.0f) {
        float jp = (t - 17.0f) / 0.6f;
        if (jp > 1.0f) jp = 1.0f;
        float bob = sinf(jp * 3.1415926f) * 42.0f;                  // small hop arc
        drawGuard(LAMA_POST_X, laneY3 + bob, LANTERN_GUARD, t, JUMP, t - 17.0f, 1.0f);
    } else {
        drawGuard(lamaX, laneY3, LANTERN_GUARD, t, guardSt, 0.0f, 1.0f, running);
    }

    // performers: walking while p<1, then standing; the clown winds up his throw
    AnimState clownSt = WALK;
    float clownStateTime = 0.0f;
    if (t >= 16.0f) { clownSt = ATTACK; clownStateTime = 0.0f; }
    if (t >= 16.2f) { clownStateTime = t - 16.2f; }                 // throw animation plays
    drawEnemy(clownX, laneY3, CLOWN, t, clownSt, clownStateTime, 1.0f);
    drawRedEyes(clownX, laneY3 + 85.0f, 1.0f);                      // eyes ride with them

    AnimState jesterSt = (chasing || (t >= 16.3f && lungeP < 1.0f)) ? WALK : ATTACK;
    float jesterStateTime = (jesterSt == ATTACK) ? t - 16.8f : 0.0f;
    drawEnemy(jesterX, laneY1, JESTER, t, jesterSt, jesterStateTime, 1.0f);
    drawRedEyes(jesterX, laneY1 + 85.0f, 1.0f);

    AnimState magSt = chasing ? WALK : ATTACK;
    float magStateTime = (magSt == ATTACK) ? t - 16.0f : 0.0f;
    drawEnemy(magX, laneY3, MAGICIAN, t, magSt, magStateTime, 1.0f);
    drawRedEyes(magX, laneY3 + 85.0f, 1.0f);
}

static void drawBeat6(float t) {
    float local = t - BEAT6_START;
    drawStageBase(t, true, true, false);
    drawBeat6Actors(t);
    // (no tape static over the stage: the chase is shown clean while the owner talks)
    // no fade here: it continues straight on from beat 4

    if (t < 12.6f)       subtitleOneLine("Owner: Hello, hello! Welcome to the carnival!");
    else if (t < 14.2f)  subtitleOneLine("Owner: Our performers... wander after closing.");
    else if (t < 16.0f)  subtitleOneLine("Owner: Don't let them reach the Golden Ticket.");
    else if (t >= 17.2f) subtitleOneLine("Jana: Stay behind me!");
    else if (t >= 16.0f) subtitleOneLine("Jester: The Golden Ticket... our way out!");
}

static void drawBeat7(float t) {
    // the stage stays until the card is gone: beat 6's actors frozen behind
    drawStageBase(t, true, true, false);
    drawBeat6Actors(t);

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

// ================= the clock =================

void startCutscene() {
    cutsceneTime = 0.0f;
    previousCutsceneTime = 0.0f;
    if (ownerTexture == 0) {   // only the first time the cutscene starts
        ownerTexture     = loadTexture(ASSET_DIR "background/Owner.png");
        shadowArmTexture = loadTexture(ASSET_DIR "background/shadow_arm_strip.png");
        ticketTexture    = loadTexture(ASSET_DIR "background/golden_ticket.png");
        posterTexture    = loadTexture(ASSET_DIR "background/job_poster.png");
        //wheelTexture     = loadTexture(ASSET_DIR "background/ferris_wheel.png");
        //visitorTexture   = loadTexture(ASSET_DIR "background/visitor.png");
        cassetteTexture  = loadTexture(ASSET_DIR "background/cassette.png");
    }
}

void updateCutscene(float dt) {
    if (dt > 0.1f) dt = 0.1f;                  // tab-out gaps shouldn't skip beats
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
    if      (cutsceneTime < BEAT2_START) drawBeat1(cutsceneTime);
    else if (cutsceneTime < BEAT3_START) drawBeat2(cutsceneTime);
    else if (cutsceneTime < BEAT4_START) drawBeat3(cutsceneTime);
    else if (cutsceneTime < BEAT5_START) drawBeat4(cutsceneTime);
    //else if (cutsceneTime < BEAT6_START) drawBeat5(cutsceneTime);
    else if (cutsceneTime < 18.5f)       drawBeat6(cutsceneTime);
    else                                 drawBeat7(cutsceneTime);
}

void skipCutscene() {
    cutsceneTime = BEAT7_END;                  // jumping straight to the game
    previousCutsceneTime = BEAT7_END;          // swallow any pending sound cues
}

bool isCutsceneFinished() { return cutsceneTime >= BEAT7_END; }
