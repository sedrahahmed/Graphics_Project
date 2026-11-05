// SoundMenu.cpp. Owner: Person A
// Volume settings (bonus). Opened from the main menu.
// The buttons already call the Sound functions; they do nothing until Sound.cpp is written.
#include "SoundMenu.h"
#include "StateManager.h"
#include "Sound.h"
#include "Button.h"
#include "Draw.h"
#include "Text.h"
#include "Colors.h"
#include "Config.h"

static const float CENTER_X = SCREEN_W / 2.0f;
static Button upButton    = { CENTER_X + 160, 330, 140, 70, "+" };               
static Button downButton  = { CENTER_X - 160, 330, 140, 70, "-" };
static Button musicButton = { CENTER_X - 150, 240, 280, 70, "MUSIC: ON" };       
static Button sfxButton   = { CENTER_X + 150, 240, 280, 70, "EFFECTS: ON" };
static Button muteAll     = { CENTER_X,       150, 580, 70, "MUSIC & EFFECTS: ON" }; 
static Button backButton  = { CENTER_X,        60, 280, 70, "BACK" };            

// The volume bar between the - and + buttons
static const float BAR_W = 160.0f, BAR_H = 24.0f;

void updateSoundMenu() {
    if (isClicked(upButton))   volumeUp();
    if (isClicked(downButton)) volumeDown();
    if (isClicked(musicButton)) toggleMusicMute();
    if( isClicked(sfxButton)) toggleSoundEffectsMute();
    if (isClicked(backButton)) changeState(MAIN_MENU);
    if(isClicked(muteAll)){
        toggleMusicMute();
        toggleSoundEffectsMute();
    }
}

void drawSoundMenu() {
    drawRect(CENTER_X, SCREEN_H / 2.0f, SCREEN_W, SCREEN_H, 0.0f,NIGHT_SKY);

    const char* title = "SOUND";
    drawText(title, CENTER_X - textWidth(title, 1.2f) / 2.0f, 480, 1.2f, TICKET_GOLD);

    // Volume bar: a dark empty bar, then a gold part filled up to the volume (0 to 1).
    // The filled part starts at the bar's LEFT edge, so its center moves as it grows.
    float volume = getVolume();
    float fillW = BAR_W * volume;
    float barLeft = CENTER_X - BAR_W / 2.0f;
    drawRect(CENTER_X, 330, BAR_W, BAR_H, 0.0f, BLACK);
    if (fillW > 0.0f) drawRect(barLeft + fillW / 2.0f, 330, fillW, BAR_H, 0.0f, TICKET_GOLD);

    // The mute button's word changes so you know what clicking it will do
    musicButton.label = isMusicMuted() ? "MUSIC: OFF" : "MUSIC: ON";
    sfxButton.label = isSoundEffectsMuted() ? "SoundEffects: OFF" : "SoundEffects: ON";
    muteAll.label = isSoundEffectsMuted()&&isMusicMuted() ? "MUSIC & EFFECTS: OFF" : "MUSIC & EFFECTS: ON";

    drawButton(downButton);
    drawButton(upButton);
    drawButton(musicButton);
    drawButton(sfxButton);
    drawButton(muteAll);
    drawButton(backButton);
}
